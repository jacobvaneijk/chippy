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

#include <stdio.h>

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

START_TEST(test_ret)
{
    struct chippy *machine = chippy_create();

    machine->stack[machine->sp++] = 0x210;
    machine->stack[machine->sp++] = 0x220;
    machine->stack[machine->sp++] = 0x230;

    ck_assert_int_eq(machine->sp, 3);

    chippy_insert_opcode(machine, 0x00EE, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x230);
    ck_assert_int_eq(machine->sp, 2);
}
END_TEST

START_TEST(test_jp)
{
    struct chippy *machine = chippy_create();

    chippy_insert_opcode(machine, 0x1234, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x234);
}
END_TEST

START_TEST(test_call)
{
    struct chippy *machine = chippy_create();

    machine->stack[machine->sp++] = 0x234;

    ck_assert_int_eq(machine->sp, 1);

    chippy_insert_opcode(machine, 0x2345, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->sp, 2);
    ck_assert_int_eq(machine->pc, 0x345);
}
END_TEST

START_TEST(test_se_xkk)
{
    struct chippy *machine = chippy_create();

    machine->V[2] = 0x12;

    chippy_insert_opcode(machine, 0x3212, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x204);
}
END_TEST

START_TEST(test_sne_xkk)
{
    struct chippy *machine = chippy_create();

    machine->V[2] = 0x12;

    chippy_insert_opcode(machine, 0x4299, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x204);
}
END_TEST

START_TEST(test_se_xy)
{
    struct chippy *machine = chippy_create();

    machine->V[2] = 0x12;
    machine->V[3] = 0x12;

    chippy_insert_opcode(machine, 0x5230, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->pc, 0x204);
}
END_TEST

START_TEST(test_ld_xy)
{
    struct chippy *machine = chippy_create();

    chippy_insert_opcode(machine, 0x6230, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[2], 0x30);
}
END_TEST

START_TEST(test_add_xkk)
{
    struct chippy *machine = chippy_create();

    chippy_insert_opcode(machine, 0x7120, 0x200);
    chippy_step(machine);

    ck_assert_int_eq(machine->V[1], 0x20);
}
END_TEST

Suite *create_opcodes_suite(void) {
    Suite *suite = suite_create("Opcodes");
    TCase *chain = tcase_create("opcode tests");

    suite_add_tcase(suite, chain);

    tcase_add_test(chain, test_cls);
    tcase_add_test(chain, test_ret);
    tcase_add_test(chain, test_jp);
    tcase_add_test(chain, test_call);
    tcase_add_test(chain, test_se_xkk);
    tcase_add_test(chain, test_sne_xkk);
    tcase_add_test(chain, test_se_xy);
    tcase_add_test(chain, test_ld_xy);
    tcase_add_test(chain, test_add_xkk);

    return suite;
}
