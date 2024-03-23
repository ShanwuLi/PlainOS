/* MIT License

Copyright (c) 2023 PlainOS

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <errno.h>
#include <types.h>
#include <kernel/task.h>
#include <kernel/list.h>
#include <kernel/semaphore.h>
#include <kernel/softtimer.h>
#include <kernel/mempool.h>
#include <kernel/semaphore_private.h>

struct softtimer_ctrlblock {
	tid_t daemon_task;
	struct list_node head;
	struct semaphore sem;
};

struct softtimer {
	struct list_node node;
	stimer_fun_t fun;
	void *priv_data;
	u32_t timing_ticks;
	bool auto_load;
};

static struct softtimer_ctrlblock softtimer_ctrlblock;

static int softtimer_daemon_task(int argc, char **argv)
{
	return 0;
}


stimer_handle_t pl_softtimer_request(void)
{
	struct softtimer *timer;

	timer = pl_mempool_malloc(g_pl_default_mempool, sizeof(struct softtimer));
	if (timer == NULL) {
		return NULL;
	}

	return (stimer_handle_t)timer;
}

int pl_softtimer_add(stimer_handle_t timer, stimer_fun_t *fun, u32_t timing_ticks,
                     bool auto_load, void *priv_data)
{
	struct softtimer *stimer = (struct softtimer *)timer;

	if (stimer == NULL || fun == NULL || timing_ticks == 0) {
		return -EFAULT;
	}

	stimer->fun = fun;
	stimer->priv_data = priv_data;
	stimer->timing_ticks = timing_ticks;
	stimer->auto_load = auto_load;

	pl_semaplore_take(&softtimer_ctrlblock.sem);
	list_add_node_at_tail(&softtimer_ctrlblock.head, &stimer->node);
	pl_semaplore_give(&softtimer_ctrlblock.sem);

	return OK;
}

int pl_softtimer_core_init(void)
{
	list_init(&softtimer_ctrlblock.head);
	pl_semaplore_init(&softtimer_ctrlblock.sem, 0);
	pl_task_create("softtimer_daemon", softtimer_daemon_task, 0,
	               PL_CFG_SOFTTIMER_DAEMON_TASK_STACK_SIZE, 0, NULL);
	return 0;
}
