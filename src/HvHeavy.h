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

#ifndef _HEAVY_H_
#define _HEAVY_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
class HeavyContextInterface;
#else
typedef struct HeavyContextInterface HeavyContextInterface;
#endif

#if HV_APPLE
#pragma mark - Heavy Context
#endif

/** Deletes a patch instance. */
void hv_delete(HeavyContextInterface *c);



#if HV_APPLE
#pragma mark - Heavy Process
#endif

/**
 * Processes one block of samples for a patch instance. The buffer format is an array of float channel arrays.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [[LLLL][RRRR]]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_process(HeavyContextInterface *c, float **inputBuffers, float **outputBuffers, int n);

/**
 * Processes one block of samples for a patch instance. The buffer format is an uninterleaved float array of channels.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [LLLLRRRR]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_processInline(HeavyContextInterface *c, float *inputBuffers, float *outputBuffers, int n);

/**
 * Processes one block of samples for a patch instance. The buffer format is an interleaved float array of channels.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [LRLRLRLR]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_processInlineInterleaved(HeavyContextInterface *c, float *inputBuffers, float *outputBuffers, int n);

/**
 * Processes one block of samples for a patch instance. The buffer format is an interleaved short array of channels.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [LRLRLRLR]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_processInlineInterleavedShort(HeavyContextInterface *c, short *inputBuffers, short *outputBuffers, int n);



#if HV_APPLE
#pragma mark - Heavy Common
#endif

typedef struct HvMessage HvMessage;

/**
 * Returns the total size in bytes of the context.
 * This value may change if tables are resized.
 */
int hv_getSize(HeavyContextInterface *c);

/** Returns the sample rate with which this context has been configured. */
double hv_getSampleRate(HeavyContextInterface *c);

/** Returns the number of input channels with which this context has been configured. */
int hv_getNumInputChannels(HeavyContextInterface *c);

/** Returns the number of output channels with which this context has been configured. */
int hv_getNumOutputChannels(HeavyContextInterface *c);

/** Set the print hook. The function is called whenever a message is sent to a print object. */
void hv_setPrintHook(HeavyContextInterface *c,
    void (*f)(HeavyContextInterface *, const char *, const char *, const HvMessage *m));

/** Returns the print hook, or NULL. */
void (*hv_getPrintHook(HeavyContextInterface *c))(HeavyContextInterface *, const char *, const char *, const HvMessage *);

/**
 * Set the send hook. The function is called whenever a message is sent to any send object.
 * Messages returned by this function should NEVER be freed. If the message must persist, call
 * hv_msg_copy() first.
 */
void hv_setSendHook(HeavyContextInterface *c,
    void (*f)(HeavyContextInterface *, const char *sendName, unsigned int sendHash, const HvMessage *));

/** Returns a 32-bit hash of any string. Returns 0 if string is NULL. */
unsigned int hv_stringToHash(const char *s);

/**
 * A convenience function to send a bang to a receiver to be processed immediately.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *          the message queue to be processed this block.
 */
bool hv_sendBangToReceiver(HeavyContextInterface *c, unsigned int receiverHash);

/**
 * A convenience function to send a float to a receiver to be processed immediately.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *          the message queue to be processed this block.
 */
bool hv_sendFloatToReceiver(HeavyContextInterface *c, unsigned int receiverHash, const float x);

/**
 * A convenience function to send a symbol to a receiver to be processed immediately.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *          the message queue to be processed this block.
 */
bool hv_sendSymbolToReceiver(HeavyContextInterface *c, unsigned int receiverHash, char *s);

/**
 * Sends a formatted message to a receiver that can be scheduled for the future.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *          the message queue to be processed this block.
 */
bool hv_sendMessageToReceiverV(HeavyContextInterface *c, unsigned int receiverHash, double delayMs, const char *format, ...);

/**
 * Sends a message to a receiver that can be scheduled for the future.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *          the message queue to be processed this block.
 */
bool hv_sendMessageToReceiver(HeavyContextInterface *c, unsigned int receiverHash, double delayMs, HvMessage *m);

/**
 * Cancels a previously scheduled message.
 *
 * @param sendMessage  May be NULL.
 */
void hv_cancelMessage(HeavyContextInterface *c, HvMessage *m, void (*sendMessage)(HeavyContextInterface *, int, const HvMessage *));

/** Returns the read-only user-assigned name of this patch. */
const char *hv_getName(HeavyContextInterface *c);

/** Sets a user-definable value. This value is never manipulated by Heavy. */
void hv_setUserData(HeavyContextInterface *c, void *userData);

/** Returns the user-defined data. */
void *hv_getUserData(HeavyContextInterface *c);

/** Returns the current patch time in milliseconds. This value may have rounding errors. */
double hv_getCurrentTime(HeavyContextInterface *c);

/** Returns the current patch time in samples. This value is always exact. */
unsigned int hv_getCurrentSample(HeavyContextInterface *c);

/** */
float hv_samplesToMilliseconds(HeavyContextInterface *c, unsigned int numSamples);

/** */
unsigned int hv_millisecondsToSamples(HeavyContextInterface *c, float ms);

/**
 * Acquire the message lock.
 *
 * This function will block until the message lock as been acquired.
 * Typical applications will not require the use of this function.
 *
 * @param c  A Heavy context.
 */
void hv_lock_acquire(HeavyContextInterface *c);

/**
 * Try to acquire the message lock.
 *
 * If the lock has been acquired, hv_lock_release() must be called to release it.
 * Typical applications will not require the use of this function.
 *
 * @param c  A Heavy context.
 *
 * @return Returns true if the lock has been acquired, false otherwise.
 */
bool hv_lock_try(HeavyContextInterface *c);

/**
 * Release the message lock.
 *
 * Typical applications will not require the use of this function.
 *
 * @param c  A Heavy context.
 */
void hv_lock_release(HeavyContextInterface *c);



#if HV_APPLE
#pragma mark - Heavy Message
#endif

typedef struct HvMessage HvMessage;

/** Returns the total size in bytes of a HvMessage with a number of elements on the heap. */
unsigned long hv_msg_getByteSize(unsigned int numElements);

/** Initialise a HvMessage structure with the number of elements and a timestamp (in samples). */
void hv_msg_init(HvMessage *m, int numElements, unsigned int timestamp);

/** Returns the number of elements in this message. */
unsigned long hv_msg_getNumElements(const HvMessage *m);

/** Returns the time at which this message exists (in samples). */
unsigned int hv_msg_getTimestamp(const HvMessage *m);

/** Set the time at which this message should be executed (in samples). */
void hv_msg_setTimestamp(HvMessage *m, unsigned int timestamp);

/** Returns true of the indexed element is a bang. False otherwise. Index is not bounds checked. */
bool hv_msg_isBang(const HvMessage *const m, int i);

/** Sets the indexed element to a bang. Index is not bounds checked. */
void hv_msg_setBang(HvMessage *m, int i);

/** Returns true of the indexed element is a float. False otherwise. Index is not bounds checked. */
bool hv_msg_isFloat(const HvMessage *const m, int i);

/** Returns the indexed element as a float value. Index is not bounds checked. */
float hv_msg_getFloat(const HvMessage *const m, int i);

/** Sets the indexed element to float value. Index is not bounds checked. */
void hv_msg_setFloat(HvMessage *m, int i, float f);

/** Returns true of the indexed element is a symbol. False otherwise. Index is not bounds checked. */
bool hv_msg_isSymbol(const HvMessage *const m, int i);

/** Returns the indexed element as a symbol value. Index is not bounds checked. */
const char *hv_msg_getSymbol(const HvMessage *const m, int i);

/** Returns true of the indexed element is a hash. False otherwise. Index is not bounds checked. */
bool hv_msg_isHash(const HvMessage *const m, int i);

/** Returns the indexed element as a hash value. Index is not bounds checked. */
unsigned int hv_msg_getHash(const HvMessage *const m, int i);

/** Sets the indexed element to symbol value. Index is not bounds checked. */
void hv_msg_setSymbol(HvMessage *m, int i, const char *s);

/**
 * Returns true if the message has the given format, in number of elements and type. False otherwise.
 * Valid element types are:
 * 'b': bang
 * 'f': float
 * 's': symbol
 *
 * For example, a message with three floats would have a format of "fff". A single bang is "b".
 * A message with two symbols is "ss". These types can be mixed and matched in any way.
 */
bool hv_msg_hasFormat(const HvMessage *const m, const char *fmt);

/**
 * Returns a basic string representation of the message.
 * The character array MUST be deallocated by the caller.
 */
char *hv_msg_toString(const HvMessage *const m);

/** Copy a message onto the stack. The message persists. */
HvMessage *hv_msg_copy(const HvMessage *const m);

/** Free a copied message. */
void hv_msg_free(HvMessage *m);



#if HV_APPLE
#pragma mark - Heavy Table
#endif

/**
 * Resizes the table to the given length. Length must be positive.
 * Existing contents are copied to the new table. Remaining space is cleared.
 * The change in byte-size of the table is returned. A value of zero indicates error.
 */
int hv_table_resize(HeavyContextInterface *c, unsigned int tableHash, unsigned int newLength);

/** Returns a pointer to the raw buffer backing this table. DO NOT free it. */
float *hv_table_getBuffer(HeavyContextInterface *c, unsigned int tableHash);

/** Returns the length of this table in samples. */
unsigned int hv_table_getLength(HeavyContextInterface *c, unsigned int tableHash);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _HEAVY_H_
