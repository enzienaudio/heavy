/**
 * Copyright (c) 2014,2015,2016 Enzien Audio Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _HEAVY_BASE_H_
#define _HEAVY_BASE_H_

#include "HvLightPipe.h"
#include "MessageQueue.h"

#define Base(_x) ((HvBase *) _x)

typedef struct HvBase {
  int numInputChannels;
  int numOutputChannels;
  double sampleRate;
  hv_uint32_t blockStartTimestamp;
  hv_size_t numBytes; // the total number of bytes allocated for this patch
  void (*f_scheduleMessageForReceiver)(struct HvBase *const, hv_uint32_t, HvMessage *);
  struct HvTable *(*f_getTableForHash)(struct HvBase *const, hv_uint32_t);
  MessageQueue mq;
  HvLightPipe msgPipe;
  hv_atomic_bool msgLock;
  void (*printHook)(double, const char *, const char *, void *);
  void (*sendHook)(double, const char *, const HvMessage *const, void *);
  char *basePath;
  void *userData;
  const char *name;
} HvBase;

/**
 * Schedule a message in the message queue according to its timestamp.
 * The copy of the message added to the queue is returned.
 */
static inline HvMessage *ctx_scheduleMessage(HvBase *_c, const HvMessage *const m,
    void (*sendMessage)(HvBase *, int, const HvMessage *), int outletIndex) {
  return mq_addMessageByTimestamp(&_c->mq, (HvMessage *) m, outletIndex, sendMessage);
}

static inline void ctx_scheduleMessageForReceiver(HvBase *const _c,
    hv_uint32_t receiverHash, HvMessage *m) {
  _c->f_scheduleMessageForReceiver(_c, receiverHash, m);
}

void ctx_scheduleMessageForReceiverV(HvBase *const _c, hv_uint32_t receiverHash,
    const hv_uint32_t timestamp, const char *format, ...);

void ctx_cancelMessage(HvBase *_c, HvMessage *m,
    void (*sendMessage)(HvBase *, int, const HvMessage *));

static inline int ctx_millisecondsToSamples(HvBase *_c, float timeInMs) {
  return (int) (timeInMs * _c->sampleRate / 1000.0);
}

static inline double ctx_samplesToMilliseconds(HvBase *_c, hv_uint32_t samples) {
  return ((double) samples) / (_c->sampleRate / 1000.0);
}

static inline double ctx_getSampleRate(HvBase *_c) {
  return _c->sampleRate;
}

static inline int ctx_getNumInputChannels(HvBase *_c) {
  return _c->numInputChannels;
}

static inline int ctx_getNumOutputChannels(HvBase *_c) {
  return _c->numOutputChannels;
}

static inline const char *ctx_getName(HvBase *_c) {
  return _c->name;
}

/** Returns the first sample of the block. */
static inline hv_uint32_t ctx_getBlockStartTimestamp(HvBase *_c) {
  return _c->blockStartTimestamp;
}

static inline void ctx_setPrintHook(HvBase *const _c, void (*f)(double,
    const char *, const char *, void *)) {
  _c->printHook = f;
}

static inline void ctx_setSendHook(HvBase *const _c, void (*f)(double, const char *, const HvMessage *const, void *)) {
  _c->sendHook = f;
}

static inline void *ctx_getUserData(HvBase *const _c) {
  return _c->userData;
}

static inline void ctx_setUserData(HvBase *const _c, void *userData) {
  _c->userData = userData;
}

void ctx_setBasePath(HvBase *const _c, const char *basePath);

static inline const char *ctx_getBasePath(HvBase *const _c) {
  return _c->basePath;
}

static inline struct HvTable *ctx_getTableForHash(HvBase *const _c, hv_uint32_t h) {
  return _c->f_getTableForHash(_c, h);
}

static inline struct HvTable *ctx_getTableForName(HvBase *const _c, const char *tableName) {
  return ctx_getTableForHash(_c, msg_symbolToHash(tableName));
}

/** Returns the total number of bytes allocated for this patch. */
static inline hv_size_t ctx_getNumBytes(HvBase *_c) {
  return _c->numBytes;
}

#endif // _HEAVY_BASE_H_
