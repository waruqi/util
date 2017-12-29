//
// Created by hujianzhe
//

#include "wait_queue.h"
#include "../syslib/error.h"

#ifdef	__cplusplus
extern "C" {
#endif

WaitQueue_t* wait_queue_init(WaitQueue_t* q, void(*deleter)(struct list_node_t*)) {
	q->m_initOk = FALSE;
	if (cslock_Create(&q->m_cslock) != EXEC_SUCCESS) {
		return NULL;
	}
	if (condition_Create(&q->m_condition) != EXEC_SUCCESS) {
		cslock_Close(&q->m_cslock);
		return NULL;
	}
	q->m_head = q->m_tail = NULL;
	q->m_forcewakeup = FALSE;
	q->m_deleter = deleter;
	q->m_initOk = TRUE;
	return q;
}

void wait_queue_push(WaitQueue_t* q, struct list_node_t* data) {
	if (!data) {
		return;
	}

	list_node_init(data);

	cslock_Enter(&q->m_cslock);

	if (q->m_tail) {
		list_node_insert_back(q->m_tail, data);
		q->m_tail = data;
	}
	else {
		q->m_head = q->m_tail = data;
		condition_WakeThread(&q->m_condition);
	}

	cslock_Leave(&q->m_cslock);
}

list_node_t* wait_queue_pop(WaitQueue_t* q, int msec, size_t expect_cnt) {
	struct list_node_t* res = NULL;
	if (0 == expect_cnt) {
		return res;
	}

	cslock_Enter(&q->m_cslock);

	while (NULL == q->m_head && !q->m_forcewakeup) {
		if (condition_Wait(&q->m_condition, &q->m_cslock, msec) == EXEC_SUCCESS) {
			continue;
		}
		assert_true(error_code() == ETIMEDOUT);
		break;
	}
	q->m_forcewakeup = FALSE;

	res = q->m_head;
	if (~0 == expect_cnt) {
		q->m_head = q->m_tail = NULL;
	}
	else {
		struct list_node_t *cur;
		for (cur = q->m_head; cur && --expect_cnt; cur = cur->next);
		if (0 == expect_cnt && cur && cur->next) {
			q->m_head = cur->next;
		}
		else {
			q->m_head = q->m_tail = NULL;
		}
	}

	cslock_Leave(&q->m_cslock);

	return res;
}

void wait_queue_weakup(WaitQueue_t* q) {
	q->m_forcewakeup = TRUE;
	condition_WakeThread(&q->m_condition);
}

static void __wait_queue_clear(WaitQueue_t* q) {
	if (q->m_deleter) {
		struct list_node_t *next, *cur;
		for (cur = q->m_head; cur; cur = next) {
			next = cur->next;
			q->m_deleter(cur);
		}
	}
	q->m_head = q->m_tail = NULL;
}
void wait_queue_clear(WaitQueue_t* q) {
	cslock_Enter(&q->m_cslock);
	__wait_queue_clear(q);
	cslock_Leave(&q->m_cslock);
}

void wait_queue_destroy(WaitQueue_t* q) {
	if (q->m_initOk) {
		__wait_queue_clear(q);
		cslock_Close(&q->m_cslock);
		condition_Close(&q->m_condition);
	}
}

#ifdef	__cplusplus
}
#endif