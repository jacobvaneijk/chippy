/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <check.h>

extern Suite *create_opcodes_suite();

int main(void) {
    SRunner *runner = srunner_create(create_opcodes_suite());

    srunner_run_all(runner, CK_NORMAL);

    int failed = srunner_ntests_failed(runner);

    srunner_free(runner);

    return failed == 0 ? 0 : 1;
}
