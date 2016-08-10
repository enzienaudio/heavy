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

void ctx_setBasePath(HvBase *const _c, const char *basePath) {
  hv_free(_c->basePath);
  if (basePath != NULL) {
    hv_size_t len = (hv_size_t) hv_strlen(basePath);
    _c->basePath = (char *) hv_malloc((len+1)*sizeof(char));
    hv_assert(_c->basePath != NULL);
    hv_strncpy(_c->basePath, basePath, len);
  }
}

void ctx_cancelMessage(HvBase *_c, HvMessage *m, void (*sendMessage)(HvBase *, int, const HvMessage *)) {
  mq_removeMessage(&_c->mq, m, sendMessage);
}

void ctx_scheduleMessageForReceiverV(HvBase *const _c, hv_uint32_t receiverHash,
    const hv_uint32_t timestamp, const char *format, ...) {
  va_list ap;
  va_start(ap, format);

  const int numElem = (int) hv_strlen(format);
  HvMessage *m = HV_MESSAGE_ON_STACK(numElem);
  msg_init(m, numElem, timestamp);
  for (int i = 0; i < numElem; i++) {
    switch (format[i]) {
      case 'b': msg_setBang(m,i); break;
      case 'f': msg_setFloat(m, i, (float) va_arg(ap, double)); break;
      case 's': msg_setSymbol(m, i, (char *) va_arg(ap, char *)); break;
      default: break;
    }
  }
  _c->f_scheduleMessageForReceiver(_c, receiverHash, m);

  va_end(ap);
}
