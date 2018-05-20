#ifndef _KEY_H_
#define _KEY_H_

#include "define.h"

enum KEY_STATE {
	NONE, DOWN, PRESS, LONG_PRESS, KEY_STATE_NUM,
};

struct key {
	int state;
	unsigned long press_time;
	volatile unsigned char *block;
	unsigned char scope;
	unsigned char down_state;
	int (*is_down)(void);
	int (*operate[KEY_STATE_NUM])(void *first, void *last);
};

struct key key_init(volatile unsigned char *block,
                    unsigned char scope,
                    unsigned char down_state,
                    int (*is_down)(void),
                    int (*none_operate)(void *first, void *last),
                    int (*down_operate)(void *first, void *last),
                    int (*press_operate)(void *first, void *last),
                    int (*long_press_operate)(void *first, void *last))
{
	struct key res;

	res.state = res.press_time = 0;
	res.block = block;
	res.scope = scope;
	res.down_state = down_state;

	res.operate[NONE] = none_operate;
	res.operate[DOWN] = down_operate;
	res.operate[PRESS] = press_operate;
	res.operate[LONG_PRESS] = long_press_operate;

	return res;
}

int key_is_down(struct key key)
{
	if (key.is_down)
		return key.is_down();
	return (*key.block & key.scope) == (key.down_state & key.scope);
}

int key_operate(struct key key, void *first, void *last)
{
	if (key.operate[key.state])
		return key.operate[key.state](first, last);
	return 0;
}

struct key key_state_move(struct key cur_key, unsigned long *time)
{
	int move[KEY_STATE_NUM][2] = {
		NONE, DOWN,
		NONE, PRESS,
		NONE, LONG_PRESS,
		NONE, PRESS,
	};
	int key_down = key_is_down(cur_key);

	if (cur_key.state == PRESS) {
		if (*time - cur_key.press_time >= TIME_1S)
			cur_key.state = move[cur_key.state][key_down];
	} else {
		if (cur_key.state == DOWN)
			cur_key.press_time = *time - TIME_1S * 3;
		else if (cur_key.state == LONG_PRESS)
			cur_key.press_time = *time - TIME_1S / 2;
		cur_key.state = move[cur_key.state][key_down];
	}

	return cur_key;
}


#endif /* _KEY_H_ */