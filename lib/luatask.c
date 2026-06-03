/*
* SPDX-FileCopyrightText: (c) 2026 Ashwani Kumar Kamal <ashwanikamal.im421@gmail.com>
* SPDX-License-Identifier: MIT OR GPL-2.0-only
*/

/***
* Linux task interface.
* @module task
*/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/sched.h>
#include <linux/sched/task.h>

#include "luatask.h"

LUNATIK_PRIVATECHECKER(luatask_check, luatask_t *,
	luaL_argcheck(L, private->task != NULL, ix, "task is not set");
);


/***
* @function comm
* @treturn string command name of the task
*/
static int luatask_comm(lua_State *L)
{
	luatask_t *ltask = luatask_check(L, 1);
	lua_pushstring(L, ltask->task->comm);
	return 1;
}

/***
* @function pid
* @treturn integer pid of the task
*/
static int luatask_pid(lua_State *L)
{
	luatask_t *ltask = luatask_check(L, 1);
	lua_pushinteger(L, ltask->task->pid);
	return 1;
}

/***
* @function tgid
* @treturn integer tgid of the task
*/
static int luatask_tgid(lua_State *L)
{
	luatask_t *ltask = luatask_check(L, 1);
	lua_pushinteger(L, ltask->task->tgid);
	return 1;
}

/***
* @function prio
* @treturn integer prio of the task
*/
static int luatask_prio(lua_State *L)
{
	luatask_t *ltask = luatask_check(L, 1);
	lua_pushinteger(L, ltask->task->prio);
	return 1;
}

/***
* @function cpu
* @treturn integer on_cpu of the task
*/
#ifdef CONFIG_SMP
static int luatask_cpu(lua_State *L)
{
	luatask_t *ltask = luatask_check(L, 1);
	lua_pushinteger(L, ltask->task->on_cpu);
	return 1;
}
#endif

static void luatask_release(void *private)
{
	luatask_t *ltask = (luatask_t *)private;
	if (ltask->task) {
		put_task_struct(ltask->task);
		ltask->task = NULL;
	}
}

static const luaL_Reg luatask_lib[] = {
	{NULL, NULL}
};

static const luaL_Reg luatask_mt[] = {
	{"__gc", lunatik_deleteobject},
	{"comm", luatask_comm},
	{"pid",  luatask_pid},
	{"tgid", luatask_tgid},
	{"prio", luatask_prio},
#ifdef CONFIG_SMP
	{"cpu",  luatask_cpu},
#endif
	{NULL, NULL}
};

LUNATIK_OPENER(task);
static const lunatik_class_t luatask_class = {
	.name    = "task",
	.methods = luatask_mt,
	.release = luatask_release,
	.opener = luaopen_task,
	.opt = LUNATIK_OPT_SOFTIRQ | LUNATIK_OPT_SINGLE,
};

lunatik_object_t *luatask_new(lua_State *L, struct task_struct *task)
{
	lunatik_require(L, &luatask_class);
	lunatik_object_t *object = lunatik_newobject(L, &luatask_class, sizeof(luatask_t), LUNATIK_OPT_NONE);
	luatask_t *ltask = (luatask_t *)object->private;

	get_task_struct(task);
	ltask->task = task;

	return object;
}
EXPORT_SYMBOL(luatask_new);

LUNATIK_CLASSES(task, &luatask_class);
LUNATIK_NEWLIB(task, luatask_lib, luatask_classes);

static int __init luatask_init(void)
{
	return 0;
}

static void __exit luatask_exit(void)
{
}

module_init(luatask_init);
module_exit(luatask_exit);
MODULE_LICENSE("Dual MIT/GPL");
MODULE_AUTHOR("Ashwani Kumar Kamal <ashwanikamal.im421@gmail.com>");

