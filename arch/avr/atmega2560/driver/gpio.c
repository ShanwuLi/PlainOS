#include <types.h>
#include <errno.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>
#include <drivers/gpio/gpio.h>

/*************************************************************************************
 * struct gpio:
 * Description:
 *   @grp_nr: number of the group.
 *   @grp: gpio group.
 *
 * NOTE:
 *   group1: PORTA: [0 1 2 3 4 5 6 7]
 *   group2: PORTB: [0 1 2 3 4 5 6 7]
 *   ...
 *   group3: PORTG: [0 1 2 3 4 5 6 7]
 *
 ************************************************************************************/
static struct gpio_grp atmega2560_gpio_grp['G' - 'A' + 1] = {
	[0] = {
		.nr = 8,
		.some = (void *)((0x20 << 8) | 0x22)
	},
	[1] = {
		.nr = 8,
		.some = (void *)((0x23 << 8) | 0x25)
	},
	[2] = {
		.nr = 8,
		.some = (void *)((0x26 << 8) | 0x28)
	},
	[3] = {
		.nr = 8,
		.some = (void *)((0x29 << 8) | 0x2b)
	},
	[4] = {
		.nr = 8,
		.some = (void *)((0x2c << 8) | 0x2e)
	},
	[5] = {
		.nr = 8,
		.some = (void *)((0x2f << 8) | 0x31)
	},
	[6] = {
		.nr = 8,
		.some = (void *)((0x32 << 8) | 0x34)
	},
};

static struct gpio atmega2560_gpio = {
	.io_nr = 6 * 8,
	.grp_nr = 6,
	.grp = atmega2560_gpio_grp
};

static int mega2560_gpio_set_one(struct gpio *gpio, u16_t io_idx, u8_t set, u8_t value)
{
	USED(set);
	u16_t grp_idx = io_idx / 8;
	u16_t idx_in_grp = io_idx & 0x07;
	struct gpio_grp *grp = &gpio->grp[grp_idx];

	switch (set) {
	case GPIO_SET_VALUE:
		if (value)
			*(volatile u8_t*)((u16_t)grp->some & 0xff) |= (u8_t)(1 << idx_in_grp);
		else
			*(volatile u8_t*)((u16_t)grp->some & 0xff) &= ~(u8_t)(1 << idx_in_grp);
		break;
	
	default:
		return -ENOSUPPORT;
	}

	return 0;
}

struct gpio_ops ops = {
	.set_one = mega2560_gpio_set_one,
	.get_one = NULL,
	.set_grp = NULL,
	.get_grp = NULL
};

static struct gpio_desc atmega2560_gpio_desc = {
	.name = "atmega2560 gpio",
	.gpio = &atmega2560_gpio,
	.ops = &ops,
};

static int atmega2560_gpio_init(void)
{
	int ret;

	ret = pl_gpio_desc_register(&atmega2560_gpio_desc);
	if (ret < 0) {
		pl_early_syslog_err("atmega2560_gpio_init failed, ret:%d\r\n", ret);
		return ret;
	}

	pl_early_syslog_info("atmega2560_gpio_init done\r\n");
	return OK;
} 
pl_bsp_initcall(atmega2560_gpio_init);
