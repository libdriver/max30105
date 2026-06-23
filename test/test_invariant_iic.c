#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Include the actual header to call the real function
#include "project/raspberrypi4b/interface/src/iic.h"

START_TEST(test_iic_send_buffer_bounds)
{
    // Invariant: iic_send must not write beyond buf_send buffer boundaries regardless of len parameter
    const struct {
        uint8_t *buf;
        size_t len;
        const char *description;
    } test_cases[] = {
        {(uint8_t[]){0xAA, 0xBB, 0xCC}, 3, "valid_normal"},
        {(uint8_t[]){0x00}, 0, "zero_length"},
        {(uint8_t[]){0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 5, "exact_buffer_size"},
        {(uint8_t[]){0xDE, 0xAD, 0xBE, 0xEF}, 100, "overflow_exploit"},
        {NULL, 0, "null_buffer"}
    };
    
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);
    
    for (int i = 0; i < num_cases; i++) {
        uint8_t buf_send[6] = {0};  // Known buffer size from context
        uint8_t canary[16] = {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42,
                              0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42};
        
        // Place canary after buffer to detect overflow
        uint8_t *test_area = malloc(sizeof(buf_send) + sizeof(canary));
        memcpy(test_area, buf_send, sizeof(buf_send));
        memcpy(test_area + sizeof(buf_send), canary, sizeof(canary));
        
        // Call the actual production function
        int result = iic_send(test_area + 1, test_cases[i].buf, test_cases[i].len);
        
        // Check canary integrity
        ck_assert_mem_eq(test_area + sizeof(buf_send), canary, sizeof(canary));
        
        free(test_area);
        
        // Additional safety: function should handle invalid parameters gracefully
        if (test_cases[i].len > sizeof(buf_send) - 1) {
            ck_assert_int_ne(result, 0);  // Should return error for overflow attempts
        }
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_iic_send_buffer_bounds);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}