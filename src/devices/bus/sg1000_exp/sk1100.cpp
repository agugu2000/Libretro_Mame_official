// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    Sega SK-1100 keyboard emulation


Release data from the Sega Retro project:

  Year: 1984    Country/region: JP    Model code: SK-1100

Though no export version of the SK-1100 itself was actually released,
the export SC-3000 keyboard is identical aside from key labels (mostly
letters with diacritics replacing katakana), and functional differences
are all handled by software.


TODO:
- SP-400 serial printer (custom 7-pin TTL-level connector)


**********************************************************************/

#include "emu.h"
#include "sk1100.h"
#include "softlist_dev.h"

#include "formats/sc3000_bit.h"



//**************************************************************************
//  DEVICE DEFINITIONS
//**************************************************************************

DEFINE_DEVICE_TYPE(SEGA_SK1100, sega_sk1100_device, "sega_sk1100", "Sega SK-1100 Keyboard")
DEFINE_DEVICE_TYPE(SEGA_SK1100E, sega_sk1100e_device, "sega_sk1100e", "Sega SK-1100 Keyboard (with diareses)")


/*-------------------------------------------------
    INPUT_PORTS( sk1100_keys )
-------------------------------------------------*/

static INPUT_PORTS_START( sk1100_keys )
	PORT_START("PA0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"1 \u30CC  !  GOTO") PORT_CODE(KEYCODE_1) PORT_CHAR('1', 0x30cc) PORT_CHAR('!') // ヌ
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"Q \u30BF  REM") PORT_CODE(KEYCODE_Q) PORT_CHAR('Q', 0x30bf) PORT_CHAR('q') // タ
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"A \u30C1  CHR$(") PORT_CODE(KEYCODE_A) PORT_CHAR('A', 0x30c1) PORT_CHAR('a') // チ
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"Z \u30C4  ASC(") PORT_CODE(KEYCODE_Z) PORT_CHAR('Z', 0x30c4) PORT_CHAR('z', 0x30c3) // ツ ッ
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u82F1\u6570/\u30AB\u30CA (Alphanumerics/Kana)") PORT_CODE(KEYCODE_RALT) PORT_CHAR(UCHAR_MAMEKEY(RALT)) // 英数/カナ
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8", \u30CD  < \u3001  SGN(") PORT_CODE(KEYCODE_COMMA) PORT_CHAR(',', 0x30cd) PORT_CHAR('<', 0x3001) // ネ 、
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"K \u30CE  COS(") PORT_CODE(KEYCODE_K) PORT_CHAR('K', 0x30ce) PORT_CHAR('k') // ノ
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"I \u30CB  TO") PORT_CODE(KEYCODE_I) PORT_CHAR('I', 0x30cb) PORT_CHAR('i') // ニ

	PORT_START("PA1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"2 \u30D5  \"  GOSUB") PORT_CODE(KEYCODE_2) PORT_CHAR('2', 0x30d5) PORT_CHAR('"') // フ
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"W \u30C6  DATA") PORT_CODE(KEYCODE_W) PORT_CHAR('W', 0x30c6) PORT_CHAR('w') // テ
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"S \u30C8  STR$(") PORT_CODE(KEYCODE_S) PORT_CHAR('S', 0x30c8) PORT_CHAR('s') // ト
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"X \u30B5  VAL(") PORT_CODE(KEYCODE_X) PORT_CHAR('X', 0x30b5) PORT_CHAR('x') // サ
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("SPC") PORT_CODE(KEYCODE_SPACE) PORT_CHAR(' ')
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8". \u30EB  > \u3002  RND(") PORT_CODE(KEYCODE_STOP) PORT_CHAR('.', 0x30eb) PORT_CHAR('>', 0x3002) // ル 。
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"L \u30EA  TAN(") PORT_CODE(KEYCODE_L) PORT_CHAR('L', 0x30ea) PORT_CHAR('l') // リ
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"O \u30E9  STEP") PORT_CODE(KEYCODE_O) PORT_CHAR('O', 0x30e9) PORT_CHAR('o') // ラ

	PORT_START("PA2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"3 \u30A2  #  RETURN") PORT_CODE(KEYCODE_3) PORT_CHAR('3', 0x30a2) PORT_CHAR('#', 0x30a1) // ア ァ
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"E \u30A4  READ") PORT_CODE(KEYCODE_E) PORT_CHAR('E', 0x30a4) PORT_CHAR('e', 0x30a3) // イ ィ
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"D \u30B7  TIME$") PORT_CODE(KEYCODE_D) PORT_CHAR('D', 0x30b7) PORT_CHAR('d') // シ
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"C \u30BD  SQR(") PORT_CODE(KEYCODE_C) PORT_CHAR('C', 0x30bd) PORT_CHAR('c') // ソ
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Clr  Home  CLS") PORT_CODE(KEYCODE_INSERT) PORT_CHAR(UCHAR_MAMEKEY(HOME))
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"/ \u30E1  ?  FRE") PORT_CODE(KEYCODE_SLASH) PORT_CHAR('/', 0x30e1) PORT_CHAR('?') // メ
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"; \u30EC  +  RAD(") PORT_CODE(KEYCODE_COLON) PORT_CHAR(';', 0x30ec) PORT_CHAR('+') // レ
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"P \u30BB  NEXT") PORT_CODE(KEYCODE_P) PORT_CHAR('P', 0x30bb) PORT_CHAR('p') // セ

	PORT_START("PA3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"4 \u30A6  $  SCREEN") PORT_CODE(KEYCODE_4) PORT_CHAR('4', 0x30a6) PORT_CHAR('$', 0x30a5) // ウ ゥ
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"R \u30B9  RESTORE") PORT_CODE(KEYCODE_R) PORT_CHAR('R', 0x30b9) PORT_CHAR('r') // ス
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"F \u30CF  LEFT$(") PORT_CODE(KEYCODE_F) PORT_CHAR('F', 0x30cf) PORT_CHAR('f') // ハ
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"V \u30D2  EXP(") PORT_CODE(KEYCODE_V) PORT_CHAR('V', 0x30d2) PORT_CHAR('v') // ヒ
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Del  Ins  DELETE") PORT_CODE(KEYCODE_DEL) PORT_CHAR(8) PORT_CHAR(UCHAR_MAMEKEY(INSERT))
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u30ED  π  PI") PORT_CODE(KEYCODE_BACKSLASH2) PORT_CHAR(0x30ed) PORT_CHAR(U'π') // ロ
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8": \u30B1  *  PRINT") PORT_CODE(KEYCODE_QUOTE) PORT_CHAR(':', 0x30b1) PORT_CHAR('*') // ケ
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"@ \u309B  `  IF") PORT_CODE(KEYCODE_OPENBRACE) PORT_CHAR('@', 0x309b) PORT_CHAR('`') // ゛

	PORT_START("PA4")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"5 \u30A8  %  POSITION") PORT_CODE(KEYCODE_5) PORT_CHAR('5', 0x30a8) PORT_CHAR('%', 0x30a7) // エ ェ
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"T \u30AB  INPUT") PORT_CODE(KEYCODE_T) PORT_CHAR('T', 0x30ab) PORT_CHAR('t') // カ
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"G \u30AD  RIGHT$(") PORT_CODE(KEYCODE_G) PORT_CHAR('G', 0x30ad) PORT_CHAR('g') // キ
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"B \u30B3  LOG(") PORT_CODE(KEYCODE_B) PORT_CHAR('B', 0x30b3) PORT_CHAR('b') // コ
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2193") PORT_CODE(KEYCODE_DOWN) PORT_CHAR(UCHAR_MAMEKEY(DOWN)) // U+2193 = ↓
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"] \u30E0  } \u300d  LIST") PORT_CODE(KEYCODE_BACKSLASH) PORT_CHAR(']', 0x30e0) PORT_CHAR('}', 0x300d) // ム 」
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"[ \u309C  { \u300c  THEN") PORT_CODE(KEYCODE_CLOSEBRACE) PORT_CHAR('[', 0x309c) PORT_CHAR('{', 0x300c) // ゜ 「

	PORT_START("PA5")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"6 \u30AA  &  COLOR") PORT_CODE(KEYCODE_6) PORT_CHAR('6', 0x30aa) PORT_CHAR('&', 0x30a9) // オ ォ
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"Y \u30F3  DIM") PORT_CODE(KEYCODE_Y) PORT_CHAR('Y', 0x30f3) PORT_CHAR('y') // ン
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"H \u30AF  MID$(") PORT_CODE(KEYCODE_H) PORT_CHAR('H', 0x30af) PORT_CHAR('h') // ク
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"N \u30DF  ABS(") PORT_CODE(KEYCODE_N) PORT_CHAR('N', 0x30df) PORT_CHAR('n') // ミ
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2190") PORT_CODE(KEYCODE_LEFT) PORT_CHAR(UCHAR_MAMEKEY(LEFT)) // U+2190 = ←
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("CR") PORT_CODE(KEYCODE_ENTER) PORT_CHAR(13)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PA6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"7 \u30E4  '  LINE") PORT_CODE(KEYCODE_7) PORT_CHAR('7', 0x30e4) PORT_CHAR('\'', 0x30e3) // ヤ ャ
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"U \u30CA  FOR") PORT_CODE(KEYCODE_U) PORT_CHAR('U', 0x30ca) PORT_CHAR('u') // ナ
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"J \u30DE  SIN(") PORT_CODE(KEYCODE_J) PORT_CHAR('J', 0x30de) PORT_CHAR('j') // マ
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"M \u30E2  INT(") PORT_CODE(KEYCODE_M) PORT_CHAR('M', 0x30e2) PORT_CHAR('m') // モ
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2192") PORT_CODE(KEYCODE_RIGHT) PORT_CHAR(UCHAR_MAMEKEY(RIGHT)) // U+2192 = →
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2191") PORT_CODE(KEYCODE_UP) PORT_CHAR(UCHAR_MAMEKEY(UP)) // U+2191 = ↑
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PA7")
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNUSED ) // keyboard disabled

	PORT_START("PB0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"8 \u30E6  (  PSET") PORT_CODE(KEYCODE_8) PORT_CHAR('8', 0x30e6) PORT_CHAR('(', 0x30e5) // ユ ュ
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"9 \u30E8  )  SAVE\"") PORT_CODE(KEYCODE_9) PORT_CHAR('9', 0x30e8) PORT_CHAR(')', 0x30e7) // ヨ ョ
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"0 \u30EF  LOAD\"") PORT_CODE(KEYCODE_0) PORT_CHAR('0', 0x30ef) // ワ
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"- \u30DB  =  VERIFY\"") PORT_CODE(KEYCODE_MINUS) PORT_CHAR('-', 0x30db) PORT_CHAR('=') // ホ
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB4")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"^ \u30D8  ~  AUTO") PORT_CODE(KEYCODE_EQUALS) PORT_CHAR('^', 0x30d8) PORT_CHAR('~') // ヘ
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB5")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"¥ \u30FC  |  RUN") PORT_CODE(KEYCODE_BACKSPACE) PORT_CHAR(U'¥', 0x30fc) PORT_CHAR('|') // ー
	PORT_BIT( 0x06, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Func") PORT_CODE(KEYCODE_TAB)

	PORT_START("PB6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Break") PORT_CODE(KEYCODE_ESC) PORT_CHAR(UCHAR_MAMEKEY(ESC))
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Graph") PORT_CODE(KEYCODE_LALT) PORT_CHAR(UCHAR_MAMEKEY(LALT))
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Ctrl") PORT_CODE(KEYCODE_LCONTROL) PORT_CHAR(UCHAR_MAMEKEY(LCONTROL))
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Shift") PORT_CODE(KEYCODE_LSHIFT) PORT_CODE(KEYCODE_RSHIFT) PORT_CHAR(UCHAR_SHIFT_1)

	PORT_START("PB7")
	PORT_BIT( 0x0f, IP_ACTIVE_LOW, IPT_UNUSED ) // keyboard disabled
INPUT_PORTS_END

static INPUT_PORTS_START( sk1100e_keys )
	PORT_START("PA0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"1  !  Â  GOTO") PORT_CODE(KEYCODE_1) PORT_CHAR('1') PORT_CHAR('!') PORT_CHAR(U'Â')
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"Q  Á  REM") PORT_CODE(KEYCODE_Q) PORT_CHAR('Q') PORT_CHAR('q') PORT_CHAR(U'Á')
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"A  Ä  CHR$(") PORT_CODE(KEYCODE_A) PORT_CHAR('A') PORT_CHAR('a') PORT_CHAR(U'Ä')
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"Z  μ  ASC(") PORT_CODE(KEYCODE_Z) PORT_CHAR('Z') PORT_CHAR('z') PORT_CHAR(U'μ')
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Eng Dier's") PORT_CODE(KEYCODE_RALT) PORT_CHAR(UCHAR_SHIFT_2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8",  <  Ū  SGN(") PORT_CODE(KEYCODE_COMMA) PORT_CHAR(',') PORT_CHAR('<') PORT_CHAR(U'Ū')
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"K  Ü  COS(") PORT_CODE(KEYCODE_K) PORT_CHAR('K') PORT_CHAR('k') PORT_CHAR(U'Ü')
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"I  Ù  TO") PORT_CODE(KEYCODE_I) PORT_CHAR('I') PORT_CHAR('i') PORT_CHAR(U'Ù')

	PORT_START("PA1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"2  \"  Ǎ  GOSUB") PORT_CODE(KEYCODE_2) PORT_CHAR('2') PORT_CHAR('"') PORT_CHAR(U'Ă')
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"W  À  DATA") PORT_CODE(KEYCODE_W) PORT_CHAR('W') PORT_CHAR('w') PORT_CHAR(U'À')
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"S  Å  STR$(") PORT_CODE(KEYCODE_S) PORT_CHAR('S') PORT_CHAR('s') PORT_CHAR(U'Å')
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"X  Λ  VAL(") PORT_CODE(KEYCODE_X) PORT_CHAR('X') PORT_CHAR('x') PORT_CHAR(U'Λ')
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("SPC") PORT_CODE(KEYCODE_SPACE) PORT_CHAR(' ')
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8".  >  Î  RND(") PORT_CODE(KEYCODE_STOP) PORT_CHAR('.') PORT_CHAR('>') PORT_CHAR(U'Î')
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"L  Í  TAN(") PORT_CODE(KEYCODE_L) PORT_CHAR('L') PORT_CHAR('l') PORT_CHAR(U'Í')
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"O  Ì  STEP") PORT_CODE(KEYCODE_O) PORT_CHAR('O') PORT_CHAR('o') PORT_CHAR(U'Ì')

	PORT_START("PA2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"3  #  Ê  RETURN") PORT_CODE(KEYCODE_3) PORT_CHAR('3') PORT_CHAR('#') PORT_CHAR(U'Ê')
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"E  É  READ") PORT_CODE(KEYCODE_E) PORT_CHAR('E') PORT_CHAR('e') PORT_CHAR(U'É')
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"D  Ã  TIME$") PORT_CODE(KEYCODE_D) PORT_CHAR('D') PORT_CHAR('d') PORT_CHAR(U'Ã')
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"C  Ç  SQR(") PORT_CODE(KEYCODE_C) PORT_CHAR('C') PORT_CHAR('c') PORT_CHAR(U'Ç')
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Clr  Home  CLS") PORT_CODE(KEYCODE_INSERT) PORT_CHAR(UCHAR_MAMEKEY(HOME))
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"/  ?  Ī  FRE") PORT_CODE(KEYCODE_SLASH) PORT_CHAR('/') PORT_CHAR('?') PORT_CHAR(U'Ī')
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8";  +  Ï  RAD(") PORT_CODE(KEYCODE_COLON) PORT_CHAR(';') PORT_CHAR('+') PORT_CHAR(U'Ï')
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"P  Ó  NEXT") PORT_CODE(KEYCODE_P) PORT_CHAR('P') PORT_CHAR('p') PORT_CHAR(U'Ó')

	PORT_START("PA3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"4  $  Ě  SCREEN") PORT_CODE(KEYCODE_4) PORT_CHAR('4') PORT_CHAR('$') PORT_CHAR(U'Ě')
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"R  È  RESTORE") PORT_CODE(KEYCODE_R) PORT_CHAR('R') PORT_CHAR('r') PORT_CHAR(U'È')
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"F  Ā  LEFT$(") PORT_CODE(KEYCODE_F) PORT_CHAR('F') PORT_CHAR('f') PORT_CHAR(U'Ā')
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"V  ¿  EXP(") PORT_CODE(KEYCODE_V) PORT_CHAR('V') PORT_CHAR('v') PORT_CHAR(U'¿')
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Del  Ins  DELETE") PORT_CODE(KEYCODE_DEL) PORT_CHAR(8) PORT_CHAR(UCHAR_MAMEKEY(INSERT))
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"Õ  π  PI") PORT_CODE(KEYCODE_BACKSLASH2) PORT_CHAR(U'Õ') PORT_CHAR(U'π')
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8":  *  Ö  PRINT") PORT_CODE(KEYCODE_QUOTE) PORT_CHAR(':') PORT_CHAR('*') PORT_CHAR(U'Ö')
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"@  `  Ò  IF") PORT_CODE(KEYCODE_OPENBRACE) PORT_CHAR('@') PORT_CHAR('`') PORT_CHAR(U'Ò')

	PORT_START("PA4")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"5  %  Ë  POSITION") PORT_CODE(KEYCODE_5) PORT_CHAR('5') PORT_CHAR('%') PORT_CHAR(U'Ë')
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"T  Ñ  INPUT") PORT_CODE(KEYCODE_T) PORT_CHAR('T') PORT_CHAR('t') PORT_CHAR(U'Ñ')
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"G  α  RIGHT$(") PORT_CODE(KEYCODE_G) PORT_CHAR('G') PORT_CHAR('g') PORT_CHAR(U'α')
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"B  ¡  LOG(") PORT_CODE(KEYCODE_B) PORT_CHAR('B') PORT_CHAR('b') PORT_CHAR(U'¡')
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2193") PORT_CODE(KEYCODE_DOWN) PORT_CHAR(UCHAR_MAMEKEY(DOWN)) // U+2193 = ↓
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"]  }  Σ  LIST") PORT_CODE(KEYCODE_BACKSLASH) PORT_CHAR(']') PORT_CHAR('}') PORT_CHAR(U'Σ')
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"[  {  Φ  THEN") PORT_CODE(KEYCODE_CLOSEBRACE) PORT_CHAR('[') PORT_CHAR('{') PORT_CHAR(U'Φ')

	PORT_START("PA5")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"6  &  Ē  COLOR") PORT_CODE(KEYCODE_6) PORT_CHAR('6') PORT_CHAR('&') PORT_CHAR(U'Ē')
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"Y  Nˆ  DIM") PORT_CODE(KEYCODE_Y) PORT_CHAR('Y') PORT_CHAR('y')
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"H  β  MID$(") PORT_CODE(KEYCODE_H) PORT_CHAR('H') PORT_CHAR('h') PORT_CHAR(U'β')
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"N  Æ  ABS(") PORT_CODE(KEYCODE_N) PORT_CHAR('N') PORT_CHAR('n') PORT_CHAR(U'Æ')
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2190") PORT_CODE(KEYCODE_LEFT) PORT_CHAR(UCHAR_MAMEKEY(LEFT)) // U+2190 = ←
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("CR") PORT_CODE(KEYCODE_ENTER) PORT_CHAR(13)
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PA6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"7  '  Û  LINE") PORT_CODE(KEYCODE_7) PORT_CHAR('7') PORT_CHAR('\'') PORT_CHAR(U'Û')
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"U  Ú  FOR") PORT_CODE(KEYCODE_U) PORT_CHAR('U') PORT_CHAR('u') PORT_CHAR(U'Ú')
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"J  θ  SIN(") PORT_CODE(KEYCODE_J) PORT_CHAR('J') PORT_CHAR('j') PORT_CHAR(U'θ')
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"M  Ω  INT(") PORT_CODE(KEYCODE_M) PORT_CHAR('M') PORT_CHAR('m') PORT_CHAR(U'Ω')
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2192") PORT_CODE(KEYCODE_RIGHT) PORT_CHAR(UCHAR_MAMEKEY(RIGHT)) // U+2192 = →
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"\u2191") PORT_CODE(KEYCODE_UP) PORT_CHAR(UCHAR_MAMEKEY(UP)) // U+2191 = ↑
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PA7")
	PORT_BIT( 0xff, IP_ACTIVE_LOW, IPT_UNUSED ) // keyboard disabled

	PORT_START("PB0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"8  (  Ǔ  PSET") PORT_CODE(KEYCODE_8) PORT_CHAR('8') PORT_CHAR('(') PORT_CHAR(U'Ǔ')
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"9  )  Ǐ  SAVE\"") PORT_CODE(KEYCODE_9) PORT_CHAR('9') PORT_CHAR(')') PORT_CHAR(U'Ǐ')
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"0  Ô  LOAD\"") PORT_CODE(KEYCODE_0) PORT_CHAR('0')
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"-  =  Ǒ  VERIFY\"") PORT_CODE(KEYCODE_MINUS) PORT_CHAR('-') PORT_CHAR('=') PORT_CHAR(U'Ǒ')
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB4")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"^  ~  O¸  AUTO") PORT_CODE(KEYCODE_EQUALS) PORT_CHAR('^') PORT_CHAR('~')
	PORT_BIT( 0x0e, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("PB5")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME(u8"¥  |  £  RUN") PORT_CODE(KEYCODE_BACKSPACE) PORT_CHAR(U'¥') PORT_CHAR('|') PORT_CHAR(U'£')
	PORT_BIT( 0x06, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Func") PORT_CODE(KEYCODE_TAB)

	PORT_START("PB6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Break") PORT_CODE(KEYCODE_ESC) PORT_CHAR(UCHAR_MAMEKEY(ESC))
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Graph") PORT_CODE(KEYCODE_LALT) PORT_CHAR(UCHAR_MAMEKEY(LALT))
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Ctrl") PORT_CODE(KEYCODE_LCONTROL) PORT_CHAR(UCHAR_MAMEKEY(LCONTROL))
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_KEYBOARD ) PORT_NAME("Shift") PORT_CODE(KEYCODE_LSHIFT) PORT_CODE(KEYCODE_RSHIFT) PORT_CHAR(UCHAR_SHIFT_1)

	PORT_START("PB7")
	PORT_BIT( 0x0f, IP_ACTIVE_LOW, IPT_UNUSED ) // keyboard disabled
INPUT_PORTS_END


//-------------------------------------------------
//  input_ports - device-specific input ports
//-------------------------------------------------

ioport_constructor sega_sk1100_device::device_input_ports() const
{
	return INPUT_PORTS_NAME( sk1100_keys );
}

ioport_constructor sega_sk1100e_device::device_input_ports() const
{
	return INPUT_PORTS_NAME( sk1100e_keys );
}


void sega_sk1100_device::device_add_mconfig(machine_config &config)
{
	/* devices */
	I8255(config, m_ppi);
	m_ppi->in_pa_callback().set(FUNC(sega_sk1100_device::ppi_pa_r));
	m_ppi->in_pb_callback().set(FUNC(sega_sk1100_device::ppi_pb_r));
	m_ppi->out_pc_callback().set(FUNC(sega_sk1100_device::ppi_pc_w));

	CASSETTE(config, m_cassette);
	m_cassette->set_formats(sc3000_cassette_formats);
	m_cassette->set_default_state(CASSETTE_STOPPED | CASSETTE_MOTOR_ENABLED | CASSETTE_SPEAKER_ENABLED);
	m_cassette->set_interface("sc3000_cass");

	SK1100_PRINTER_PORT(config, m_printer_port, sk1100_printer_port_devices, nullptr);

	/* software lists */
	SOFTWARE_LIST(config, "sc3k_cart_list").set_original("sc3000_cart");
	SOFTWARE_LIST(config, "cass_list").set_original("sc3000_cass");
}

//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  sega_sk1100_device - constructor
//-------------------------------------------------

sega_sk1100_device::sega_sk1100_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock) :
	device_t(mconfig, type, tag, owner, clock),
	device_sg1000_expansion_slot_interface(mconfig, *this),
	m_cassette(*this, "cassette"),
	m_ppi(*this, UPD9255_0_TAG),
	m_printer_port(*this, "printer"),
	m_pa(*this, "PA%u", 0U),
	m_pb(*this, "PB%u", 0U),
	m_keylatch(0)
{
}

sega_sk1100_device::sega_sk1100_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	sega_sk1100_device(mconfig, SEGA_SK1100, tag, owner, clock)
{
}

sega_sk1100e_device::sega_sk1100e_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	sega_sk1100_device(mconfig, SEGA_SK1100E, tag, owner, clock)
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void sega_sk1100_device::device_start()
{
	/* register for state saving */
	save_item(NAME(m_keylatch));
}


//-------------------------------------------------
//  peripheral_r - keyboard read
//-------------------------------------------------

uint8_t sega_sk1100_device::peripheral_r(offs_t offset)
{
	return m_ppi->read(offset & 0x03);
}


//-------------------------------------------------
//  peripheral_w - keyboard write
//-------------------------------------------------

void sega_sk1100_device::peripheral_w(offs_t offset, uint8_t data)
{
	m_ppi->write(offset & 0x03, data);
}


bool sega_sk1100_device::is_readable(uint8_t offset)
{
	return (m_keylatch != 0x07 ? true : false);
}


/*-------------------------------------------------
    I8255 INTERFACE
-------------------------------------------------*/

uint8_t sega_sk1100_device::ppi_pa_r()
{
	/*
	    Signal  Description

	    PA0     Keyboard input
	    PA1     Keyboard input
	    PA2     Keyboard input
	    PA3     Keyboard input
	    PA4     Keyboard input
	    PA5     Keyboard input
	    PA6     Keyboard input
	    PA7     Keyboard input
	*/

	return m_pa[m_keylatch]->read();
}

uint8_t sega_sk1100_device::ppi_pb_r()
{
	/*
	    Signal  Description

	    PB0     Keyboard input
	    PB1     Keyboard input
	    PB2     Keyboard input
	    PB3     Keyboard input
	    PB4     /CONT input from cartridge terminal B-11
	    PB5     /FAULT input from printer
	    PB6     BUSY input from printer
	    PB7     Cassette tape input
	*/

	/* keyboard */
	uint8_t data = m_pb[m_keylatch]->read();

	/* cartridge contact */
	data |= 0x10;

	/* printer port */
	data |= m_printer_port->fault_r() << 5;
	data |= m_printer_port->busy_r() << 6;

	/* tape input */
	if (m_cassette->input() > +0.0) data |= 0x80;

	return data;
}

void sega_sk1100_device::ppi_pc_w(uint8_t data)
{
	/*
	    Signal  Description

	    PC0     Keyboard raster output
	    PC1     Keyboard raster output
	    PC2     Keyboard raster output
	    PC3     not connected
	    PC4     Cassette tape output
	    PC5     DATA to printer
	    PC6     /RESET to printer
	    PC7     /FEED to printer
	*/

	/* keyboard */
	m_keylatch = data & 0x07;

	/* cassette */
	m_cassette->output(BIT(data, 4) ? +1.0 : -1.0);

	/* printer port */
	m_printer_port->data_w(BIT(data, 5));
	m_printer_port->reset_w(BIT(data, 6));
	m_printer_port->feed_w(BIT(data, 7));
}
