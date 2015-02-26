#include <cstring>
#include <iconv.h>
#include <canna/RK.h>
#include "nodecanna.h"

#define WORKBUFSIZE (1024)

static int euc2utf8(char *instr)
{
  char workbuf[CANNA_BUFSIZE];
  char *in_p = instr;
  char *out_p = workbuf;
  size_t inlen = CANNA_BUFSIZE, outlen = CANNA_BUFSIZE;
  iconv_t cd;

  memset(workbuf, 0, CANNA_BUFSIZE);

  cd = iconv_open("UTF-8", "EUC-JP");
  iconv(cd, &in_p, &inlen, &out_p, &outlen);
  iconv_close(cd);

  strncpy(instr, workbuf, CANNA_BUFSIZE);

  return 0;
}


CannaSession::CannaSession()
{
  memset(this->kana, 0, CANNA_BUFSIZE);
  memset(this->kanj, 0, CANNA_BUFSIZE);

  jrKanjiControl(0, KC_INITIALIZE, 0);

  jrKanjiStatusWithValue ksv;
  ksv.ks = &(this->ks);
  ksv.buffer = (unsigned char*)this->kana;
  ksv.bytes_buffer = CANNA_BUFSIZE;
  ksv.val = CANNA_MODE_HenkanMode;
  jrKanjiControl(0, KC_CHANGEMODE, (char*)&ksv);
}

CannaSession::~CannaSession()
{
  jrKanjiControl(0, KC_FINALIZE, 0);
}

int CannaSession::convert(char *inromaji)
{
  int i, j, inlen, nbytes = 0, nbunsetsu = 0, nstrlen = 0;
  char workbuf[WORKBUFSIZE];

  memset(this->kanj, 0, CANNA_BUFSIZE);

  inlen = strlen(inromaji) + 1;
  strncat(inromaji, "\n", 1);
  for (i = 0; i < inlen; i++) {
    nbytes = jrKanjiString(0, *(inromaji + i), this->kana, CANNA_BUFSIZE, &(this->ks));
    if (nbytes > 0) {
      this->kana[nbytes] = '\0';

      nbunsetsu = RkBgnBun(0, this->kana, nbytes, (RK_XFER << RK_XFERBITS) | RK_KFER);
      for (j = 0; j < nbunsetsu; j++) {
        RkGoTo(0, j);
        RkGetKanji(0, (unsigned char*)workbuf, WORKBUFSIZE);
        strncat(this->kanj, workbuf, (CANNA_BUFSIZE - (nstrlen + 1)));
        strncat(this->kanj, " ", 1);
        nstrlen = strlen(this->kanj + 1);
      }
      RkEndBun(0, 0);
    }
  }

  euc2utf8(this->kana);
  euc2utf8(this->kanj);

  return(nstrlen);
}
