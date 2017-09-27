/**
 * This file is part of Chippy.
 *
 * (c) Jacob van Eijk <jacob.vaneijk@gmail.com>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <check.h>
#include <libchippy/chippy.h>
#include <stdint.h>
#include <stdlib.h>

static struct chippy *chippy_create(void) {
    struct chippy *machine = malloc(sizeof(struct chippy));

    chippy_init(machine);

    return machine;
}

static void chippy_insert_opcode(struct chippy *machine, uint16_t opcode, uint16_t address) {
    machine->ram[address] = opcode >> 8;
    machine->ram[address + 1] = opcode & 0xFF;
}

START_TEST(test_cls)
{
    struct chippy *machine = chippy_create();

    memset(machine->gfx, 1, SCREEN_W * SCREEN_H);
    chippy_insert_opcode(machine, 0x00E0, 0x200);

    chippy_step(machine);

    for (int i = 0; i < (SCREEN_W * SCREEN_H); i++) {
        ck_assert_int_eq(machine->gfx[i], 0);
    }
}
END_TEST

Suite *create_opcodes_suite(void) {
    Suite *suite = suite_create("Opcodes");
    TCase *chain = tcase_create("opcode tests");

    suite_add_tcase(suite, chain);

    tcase_add_test(chain, test_cls);

    return suite;
}
