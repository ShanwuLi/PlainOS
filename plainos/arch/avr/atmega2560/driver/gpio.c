#include <types.h>
#include <errno.h>
#include <kernel/syslog.h>
#include <kernel/initcall.h>
#include <drivers/gpio/gpio.h>


static u8_t goio_base[7][4] = {
	{	/* PINA, DDRA, PORTA, value */
		0x20, 0x21, 0x22, 0
	},
	{	/* PINB, DDRB, PORTB, value */
		0x23, 0x24, 0x25, 0
	},
	{	/* PINC, DDRC, PORTC, value */
		0x26, 0x27, 0x28, 0
	},
	{	/* PIND, DDRD, PORTD, value */
		0x29, 0x2A, 0x2B, 0
	},
	{	/* PINE, DDRE, PORTE, value */
		0x2C, 0x2D, 0x2E, 0
	},
	{	/* PINF, DDRF, PORTF, value */
		0x2F, 0x30, 0x31, 0
	},
	{	/* PING, DDRG, PORTG, value */
		0x32, 0x33, 0x34, 0
	}
};

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
		.some = (void *)goio_base[0]
	},
	[1] = {
		.nr = 8,
		.some = (void *)goio_base[1]
	},
	[2] = {
		.nr = 8,
		.some = (void *)goio_base[2]
	},
	[3] = {
		.nr = 8,
		.some = (void *)goio_base[3]
	},
	[4] = {
		.nr = 8,
		.some = (void *)goio_base[4]
	},
	[5] = {
		.nr = 8,
		.some = (void *)goio_base[5]
	},
	[6] = {
		.nr = 8,
		.some = (void *)goio_base[6]
	},
};

static struct gpio atmega2560_gpio = {
	.io_nr = 7 * 8,
	.grp_nr = 7,
	.grp = atmega2560_gpio_grp
};


static int mega2560_gpio_set_grp(struct gpio *gpio, u16_t grp_idx, u8_t set, uintptr_t value)
{
	volatile u8_t *base;
	struct gpio_grp *grp = &gpio->grp[grp_idx];

	switch (set) {
	case GPIO_SET_VALUE:
		base = (volatile u8_t *)(u16_t)(((u8_t *)(grp->some))[2]);
		*base = (u8_t)value;
		break;

	case GPIO_SET_DIRECT:
		base = (volatile u8_t *)(u16_t)(((u8_t *)(grp->some))[1]);
		*base = (u8_t)value;
		break;

	default:
		return -ENOSUPPORT;
	}

	return 0;
}

static int mega2560_gpio_set_one(struct gpio *gpio, u16_t io_idx, u8_t set, u8_t value)
{
	USED(set);
	volatile u8_t *base;
	u16_t grp_idx = io_idx / 8;
	u16_t idx_in_grp = io_idx & 0x07;
	struct gpio_grp *grp = &gpio->grp[grp_idx];

	switch (set) {
	case GPIO_SET_VALUE:
		base = (volatile u8_t *)(u16_t)(((u8_t *)(grp->some))[2]);
		if (value)
			*base |= (u8_t)(1 << idx_in_grp);
		else
			*base &= ~(u8_t)(1 << idx_in_grp);
		break;

	case GPIO_SET_DIRECT:
		base = (volatile u8_t *)(u16_t)(((u8_t *)(grp->some))[1]);
		if (value == GPIO_DIRECT_OUTPUT)
			*base |= (u8_t)(1 << idx_in_grp);
		else if (value == GPIO_DIRECT_INPUT)
			*base &= ~(u8_t)(1 << idx_in_grp);
		break;

	default:
		return -ENOSUPPORT;
	}

	return 0;
}

struct gpio_ops ops = {
	.set_one = mega2560_gpio_set_one,
	.get_one = NULL,
	.set_grp = mega2560_gpio_set_grp,
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
