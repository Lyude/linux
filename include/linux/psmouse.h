#ifndef _LINUX_PSMOUSE_H
#define _LINUX_PSMOUSE_H

#include <linux/workqueue.h>
#include <linux/device.h>
#include <linux/libps2.h>

enum psmouse_state {
	PSMOUSE_IGNORE,
	PSMOUSE_INITIALIZING,
	PSMOUSE_RESYNCING,
	PSMOUSE_CMD_MODE,
	PSMOUSE_ACTIVATED,
};

/* psmouse protocol handler return codes */
typedef enum {
	PSMOUSE_BAD_DATA,
	PSMOUSE_GOOD_DATA,
	PSMOUSE_FULL_PACKET
} psmouse_ret_t;

enum psmouse_scale {
	PSMOUSE_SCALE11,
	PSMOUSE_SCALE21
};

struct psmouse {
	void *private;
	struct input_dev *dev;
	struct ps2dev ps2dev;
	struct delayed_work resync_work;
	char *vendor;
	char *name;
	unsigned char packet[8];
	unsigned char badbyte;
	unsigned char pktcnt;
	unsigned char pktsize;
	unsigned char type;
	bool ignore_parity;
	bool acks_disable_command;
	unsigned int model;
	unsigned long last;
	unsigned long out_of_sync_cnt;
	unsigned long num_resyncs;
	enum psmouse_state state;
	char devname[64];
	char phys[32];

	unsigned int rate;
	unsigned int resolution;
	unsigned int resetafter;
	unsigned int resync_time;
	bool smartscroll;	/* Logitech only */

	psmouse_ret_t (*protocol_handler)(struct psmouse *psmouse);
	void (*set_rate)(struct psmouse *psmouse, unsigned int rate);
	void (*set_resolution)(struct psmouse *psmouse, unsigned int resolution);
	void (*set_scale)(struct psmouse *psmouse, enum psmouse_scale scale);

	int (*reconnect)(struct psmouse *psmouse);
	void (*disconnect)(struct psmouse *psmouse);
	void (*cleanup)(struct psmouse *psmouse);
	int (*poll)(struct psmouse *psmouse);

	void (*pt_activate)(struct psmouse *psmouse);
	void (*pt_deactivate)(struct psmouse *psmouse);
};

enum psmouse_type {
	PSMOUSE_NONE = 0,
	PSMOUSE_PS2,
	PSMOUSE_PS2PP,
	PSMOUSE_THINKPS,
	PSMOUSE_GENPS,
	PSMOUSE_IMPS,
	PSMOUSE_IMEX,
	PSMOUSE_SYNAPTICS,
	PSMOUSE_ALPS,
	PSMOUSE_LIFEBOOK,
	PSMOUSE_TRACKPOINT,
	PSMOUSE_TOUCHKIT_PS2,
	PSMOUSE_CORTRON,
	PSMOUSE_HGPK,
	PSMOUSE_ELANTECH,
	PSMOUSE_FSP,
	PSMOUSE_SYNAPTICS_RELATIVE,
	PSMOUSE_CYPRESS,
	PSMOUSE_FOCALTECH,
	PSMOUSE_VMMOUSE,
	PSMOUSE_AUTO		/* This one should always be last */
};

#endif /* _LINUX_PSMOUSE_H */
