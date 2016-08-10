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

#include "HvBase.h"
#include "HvTable.h"

#if !HV_WIN
#pragma mark - Heavy Table
#endif

#ifdef __cplusplus
extern "C" {
#endif
HV_EXPORT int hv_table_resize(struct HvTable *o, hv_uint32_t newLength) {
  return hTable_resize(o, newLength);
}

HV_EXPORT float *hv_table_getBuffer(struct HvTable *o) {
  return hTable_getBuffer(o);
}

HV_EXPORT hv_uint32_t hv_table_getLength(struct HvTable *o) {
  return hTable_getLength(o);
}
#ifdef __cplusplus
}
#endif



#if !HV_WIN
#pragma mark - Heavy Message
#endif

#ifdef __cplusplus
extern "C" {
#endif
HV_EXPORT hv_size_t hv_msg_getByteSize(hv_uint32_t numElements) {
  return msg_getCoreSize(numElements);
}

HV_EXPORT void hv_msg_init(HvMessage *m, int numElements, hv_uint32_t timestamp) {
  msg_init(m, numElements, timestamp);
}

HV_EXPORT hv_size_t hv_msg_getNumElements(const HvMessage *const m) {
  return msg_getNumElements(m);
}

HV_EXPORT double hv_msg_getTimestamp(const HvMessage *const m) {
  return msg_getTimestamp(m);
}

HV_EXPORT void hv_msg_setTimestamp(HvMessage *m, hv_uint32_t timestamp) {
  msg_setTimestamp(m, timestamp);
}

HV_EXPORT bool hv_msg_isBang(const HvMessage *const m, int i) {
  return msg_isBang(m,i);
}

HV_EXPORT void hv_msg_setBang(HvMessage *m, int i) {
  msg_setBang(m,i);
}

HV_EXPORT bool hv_msg_isFloat(const HvMessage *const m, int i) {
  return msg_isFloat(m, i);
}

HV_EXPORT float hv_msg_getFloat(const HvMessage *const m, int i) {
  return msg_getFloat(m,i);
}

HV_EXPORT void hv_msg_setFloat(HvMessage *m, int i, float f) {
  msg_setFloat(m,i,f);
}

HV_EXPORT bool hv_msg_isSymbol(const HvMessage *const m, int i) {
  return msg_isSymbol(m,i);
}

HV_EXPORT char *hv_msg_getSymbol(const HvMessage *const m, int i) {
  return msg_getSymbol(m,i);
}

HV_EXPORT void hv_msg_setSymbol(HvMessage *m, int i, char *s) {
  msg_setSymbol(m,i,s);
}

HV_EXPORT bool hv_msg_isHash(const HvMessage *const m, int i) {
  return msg_isHash(m, i);
}

HV_EXPORT unsigned int hv_msg_stringToHash(const char *s) {
  return msg_symbolToHash(s);
}

HV_EXPORT hv_uint32_t hv_msg_getHash(const HvMessage *const m, int i) {
  return msg_getHash(m, i);
}

HV_EXPORT bool hv_msg_hasFormat(const HvMessage *const m, const char *fmt) {
  return msg_hasFormat(m, fmt);
}

HV_EXPORT char *hv_msg_toString(const HvMessage *const m) {
  return msg_toString(m);
}

HV_EXPORT HvMessage *hv_msg_copy(HvMessage *m) {
  return msg_copy(m);
}

HV_EXPORT void hv_msg_free(HvMessage *m) {
  msg_free(m);
}
#ifdef __cplusplus
}
#endif



#if !HV_WIN
#pragma mark - Heavy Common
#endif

#ifdef __cplusplus
extern "C" {
#endif
HV_EXPORT int hv_getSize(HvBase *c) {
  return (int) ctx_getNumBytes(c);
}

HV_EXPORT double hv_getSampleRate(HvBase *c) {
  return ctx_getSampleRate(c);
}

HV_EXPORT int hv_getNumInputChannels(HvBase *c) {
  return ctx_getNumInputChannels(c);
}

HV_EXPORT int hv_getNumOutputChannels(HvBase *c) {
  return ctx_getNumOutputChannels(c);
}

HV_EXPORT const char *hv_getName(HvBase *c) {
  return ctx_getName(c);
}

HV_EXPORT void hv_setPrintHook(HvBase *c,
    void (*f)(double, const char *, const char *, void *)) {
  ctx_setPrintHook(c, f);
}

HV_EXPORT void hv_setSendHook(HvBase *c,
    void (*f)(double, const char *, const HvMessage *const, void *)) {
  ctx_setSendHook(c, f);
}

HV_EXPORT hv_uint32_t hv_stringToHash(const char *s) {
  return msg_symbolToHash(s);
}

HV_EXPORT bool hv_sendBangToReceiver(HvBase *c, hv_uint32_t receiverHash) {
  hv_assert(c != NULL);

  HV_SPINLOCK_ACQUIRE(c->msgLock);
  ReceiverMessagePair *p = (ReceiverMessagePair *) hLp_getWriteBuffer(&c->msgPipe, sizeof(ReceiverMessagePair));
  if (p != NULL) {
    p->receiverHash = receiverHash;
    msg_initWithBang(&p->msg, c->blockStartTimestamp);
    hLp_produce(&c->msgPipe, sizeof(ReceiverMessagePair));
    HV_SPINLOCK_RELEASE(c->msgLock);
    return true;
  } else {
    HV_SPINLOCK_RELEASE(c->msgLock);
    hv_assert(false &&
        "The message queue is full and cannot accept more messages until they "
        "have been processed. Try increasing the queue size in the new_with_options() constructor.");
    return false;
  }
}

HV_EXPORT bool hv_sendFloatToReceiver(HvBase *c, hv_uint32_t receiverHash, const float x) {
  hv_assert(c != NULL);

  HV_SPINLOCK_ACQUIRE(c->msgLock);
  ReceiverMessagePair *p = (ReceiverMessagePair *) hLp_getWriteBuffer(&c->msgPipe, sizeof(ReceiverMessagePair));
  if (p != NULL) {
    p->receiverHash = receiverHash;
    msg_initWithFloat(&p->msg, c->blockStartTimestamp, x);
    hLp_produce(&c->msgPipe, sizeof(ReceiverMessagePair));
    HV_SPINLOCK_RELEASE(c->msgLock);
    return true;
  } else {
    HV_SPINLOCK_RELEASE(c->msgLock);
    hv_assert(false &&
        "The message queue is full and cannot accept more messages until they "
        "have been processed. Try increasing the queue size in the new_with_options() constructor.");
    return false;
  }
}

HV_EXPORT bool hv_sendSymbolToReceiver(HvBase *c, hv_uint32_t receiverHash, char *s) {
  hv_assert(c != NULL);
  hv_assert(s != NULL);

  const hv_size_t slen = hv_strlen(s);
  hv_uint32_t numBytes = (hv_uint32_t) (sizeof(ReceiverMessagePair) + slen + 1);
  HV_SPINLOCK_ACQUIRE(c->msgLock);
  ReceiverMessagePair *p = (ReceiverMessagePair *) hLp_getWriteBuffer(&c->msgPipe, numBytes);
  if (p != NULL) {
    p->receiverHash = receiverHash;
    msg_init(&p->msg, 1, c->blockStartTimestamp);
    hv_strncpy(((char *) &p->msg)+sizeof(HvMessage), s, slen+1);
    msg_setSymbol(&p->msg, 0, ((char *) &p->msg)+sizeof(HvMessage));
    hLp_produce(&c->msgPipe, numBytes);
    HV_SPINLOCK_RELEASE(c->msgLock);
    return true;
  } else {
    HV_SPINLOCK_RELEASE(c->msgLock);
    hv_assert(false &&
        "The message queue is full and cannot accept more messages until they "
        "have been processed. Try increasing the queue size in the new_with_options() constructor.");
    return false;
  }
}

HV_EXPORT bool hv_vscheduleMessageForReceiver(
    HvBase *c, hv_uint32_t receiverHash, double delayMs, const char *format, ...) {
  hv_assert(c != NULL);
  hv_assert(delayMs >= 0.0);
  hv_assert(format != NULL);

  va_list ap;
  va_start(ap, format);
  const int numElem = (int) hv_strlen(format);
  HvMessage *m = HV_MESSAGE_ON_STACK(numElem);
  msg_init(m, numElem, c->blockStartTimestamp + (hv_uint32_t) (hv_max_d(0.0, delayMs)*ctx_getSampleRate(c)/1000.0));
  for (int i = 0; i < numElem; i++) {
    switch (format[i]) {
      case 'b': msg_setBang(m, i); break;
      case 'f': msg_setFloat(m, i, (float) va_arg(ap, double)); break;
      case 'h': msg_setHash(m, i, (int) va_arg(ap, int)); break;
      case 's': msg_setSymbol(m, i, (char *) va_arg(ap, char *)); break;
      default: break;
    }
  }
  va_end(ap);

  HV_SPINLOCK_ACQUIRE(c->msgLock);
  const hv_uint32_t numBytes = sizeof(ReceiverMessagePair) + msg_getSize(m) - sizeof(HvMessage);
  ReceiverMessagePair *p = (ReceiverMessagePair *) hLp_getWriteBuffer(&c->msgPipe, numBytes);
  if (p != NULL) {
    p->receiverHash = receiverHash;
    msg_copyToBuffer(m, (char *) &p->msg, msg_getSize(m));
    hLp_produce(&c->msgPipe, numBytes);
    HV_SPINLOCK_RELEASE(c->msgLock);
    return true;
  } else {
    HV_SPINLOCK_RELEASE(c->msgLock);
    hv_assert(false &&
        "The message queue is full and cannot accept more messages until they "
        "have been processed. Try increasing the queue size in the new_with_options() constructor.");
    return false;
  }
}

HV_EXPORT bool hv_scheduleMessageForReceiver(
    HvBase *c, hv_uint32_t receiverHash, double delayMs, HvMessage *m) {
  hv_assert(c != NULL);
  hv_assert(delayMs >= 0.0);
  hv_assert(m != NULL);

  HV_SPINLOCK_ACQUIRE(c->msgLock);
  const hv_uint32_t numBytes = sizeof(ReceiverMessagePair) + msg_getSize(m) - sizeof(HvMessage);
  ReceiverMessagePair *p = (ReceiverMessagePair *) hLp_getWriteBuffer(&c->msgPipe, numBytes);
  if (p != NULL) {
    p->receiverHash = receiverHash;
    msg_copyToBuffer(m, (char *) &p->msg, msg_getSize(m));
    msg_setTimestamp(&p->msg, c->blockStartTimestamp + (hv_uint32_t) (hv_max_d(0.0, delayMs)*(ctx_getSampleRate(c)/1000.0)));
    hLp_produce(&c->msgPipe, numBytes);
    HV_SPINLOCK_RELEASE(c->msgLock);
    return true;
  } else {
    HV_SPINLOCK_RELEASE(c->msgLock);
    hv_assert(false &&
        "The message queue is full and cannot accept more messages until they "
        "have been processed. Try increasing the queue size in the new_with_options() constructor.");
    return false;
  }
}

HV_EXPORT HvTable *hv_getTableForName(HvBase *c, const char *tableName) {
  return ctx_getTableForName(c, tableName);
}

HV_EXPORT void hv_cancelMessage(HvBase *c, HvMessage *m) {
  ctx_cancelMessage(c, m, NULL);
}

HV_EXPORT double hv_getCurrentTime(HvBase *c) {
  return ((double) c->blockStartTimestamp)/(c->sampleRate/1000.0);
}

HV_EXPORT hv_uint32_t hv_getCurrentSample(HvBase *c) {
  return c->blockStartTimestamp;
}

HV_EXPORT void *hv_getUserData(HvBase *c) {
  return ctx_getUserData(c);
}

HV_EXPORT void hv_setUserData(HvBase *c, void *userData) {
  ctx_setUserData(c, userData);
}

HV_EXPORT void hv_setBasePath(HvBase *c, const char *basePath) {
  ctx_setBasePath(c, basePath);
}
#ifdef __cplusplus
}
#endif
