
#include "fonts.h"

// Processed using
// https://lvgl.io/tools/font_conv_v5_3

using Fonts::FontCharacter;

/*Store the image of the letters (glyph)*/
static const uint8_t bitmap[] = 
{
  /*Unicode: U+0021 (!) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x00,  //. 
  0x80,  //% 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0022 (") , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0xa0,  //%.% 
  0xa0,  //%.% 
  0xa0,  //%.% 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+0023 (#) , Width: 6 */
  0x00,  //...... 
  0x00,  //...... 
  0x00,  //...... 
  0x48,  //.%..%. 
  0x48,  //.%..%. 
  0xfc,  //%%%%%% 
  0x48,  //.%..%. 
  0x48,  //.%..%. 
  0x48,  //.%..%. 
  0xfc,  //%%%%%% 
  0x48,  //.%..%. 
  0x48,  //.%..%. 
  0x00,  //...... 
  0x00,  //...... 
  0x00,  //...... 
  0x00,  //...... 


  /*Unicode: U+0024 ($) , Width: 5 */
  0x00,  //..... 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x70,  //.%%%. 
  0xa8,  //%.%.% 
  0xa0,  //%.%.. 
  0xa0,  //%.%.. 
  0x70,  //.%%%. 
  0x28,  //..%.% 
  0x28,  //..%.% 
  0xa8,  //%.%.% 
  0x70,  //.%%%. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0025 (%) , Width: 7 */
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x40,  //.%..... 
  0xa0,  //%.%.... 
  0xa4,  //%.%..%. 
  0x48,  //.%..%.. 
  0x10,  //...%... 
  0x24,  //..%..%. 
  0x4a,  //.%..%.% 
  0x0a,  //....%.% 
  0x04,  //.....%. 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 


  /*Unicode: U+0026 (&) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x80,  //%.... 
  0x80,  //%.... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0027 (') , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+0028 (() , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x20,  //..% 
  0x40,  //.%. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x40,  //.%. 
  0x20,  //..% 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+0029 ()) , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x80,  //%.. 
  0x40,  //.%. 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x40,  //.%. 
  0x80,  //%.. 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+002a (*) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x20,  //..%.. 
  0xa8,  //%.%.% 
  0x70,  //.%%%. 
  0xa8,  //%.%.% 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+002b (+) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0xf8,  //%%%%% 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+002c (,) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x40,  //.% 
  0x40,  //.% 
  0x80,  //%. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+002d (-) , Width: 4 */
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0xf0,  //%%%% 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 


  /*Unicode: U+002e (.) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x80,  //% 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+002f (/) , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x40,  //.%. 
  0x40,  //.%. 
  0x40,  //.%. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+0030 (0) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x98,  //%..%% 
  0xa8,  //%.%.% 
  0xc8,  //%%..% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0031 (1) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x20,  //..%.. 
  0x60,  //.%%.. 
  0xa0,  //%.%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0032 (2) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x08,  //....% 
  0x10,  //...%. 
  0x20,  //..%.. 
  0x40,  //.%... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0033 (3) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x08,  //....% 
  0x08,  //....% 
  0x30,  //..%%. 
  0x08,  //....% 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0034 (4) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x08,  //....% 
  0x18,  //...%% 
  0x28,  //..%.% 
  0x48,  //.%..% 
  0x88,  //%...% 
  0xf8,  //%%%%% 
  0x08,  //....% 
  0x08,  //....% 
  0x08,  //....% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0035 (5) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf0,  //%%%%. 
  0x08,  //....% 
  0x08,  //....% 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0036 (6) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0037 (7) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x08,  //....% 
  0x08,  //....% 
  0x10,  //...%. 
  0x20,  //..%.. 
  0x40,  //.%... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0038 (8) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0039 (9) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x08,  //....% 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+003a (:) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x80,  //% 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x80,  //% 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


  /*Unicode: U+003b (;) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x40,  //.% 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x40,  //.% 
  0x40,  //.% 
  0x80,  //%. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+003c (<) , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x20,  //..% 
  0x40,  //.%. 
  0x80,  //%.. 
  0x40,  //.%. 
  0x20,  //..% 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+003d (=) , Width: 4 */
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0xf0,  //%%%% 
  0x00,  //.... 
  0xf0,  //%%%% 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 


  /*Unicode: U+003e (>) , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x80,  //%.. 
  0x40,  //.%. 
  0x20,  //..% 
  0x40,  //.%. 
  0x80,  //%.. 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+003f (?) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x08,  //....% 
  0x10,  //...%. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x00,  //..... 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0040 (@) , Width: 7 */
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x7c,  //.%%%%%. 
  0x82,  //%.....% 
  0x9a,  //%..%%.% 
  0xaa,  //%.%.%.% 
  0xaa,  //%.%.%.% 
  0xaa,  //%.%.%.% 
  0x9c,  //%..%%%. 
  0x80,  //%...... 
  0x7c,  //.%%%%%. 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 


  /*Unicode: U+0041 (A) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf8,  //%%%%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0042 (B) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf0,  //%%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0043 (C) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0044 (D) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf0,  //%%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0045 (E) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xe0,  //%%%.. 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0046 (F) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xe0,  //%%%.. 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0047 (G) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x80,  //%.... 
  0x80,  //%.... 
  0x98,  //%..%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0048 (H) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf8,  //%%%%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0049 (I) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+004a (J) , Width: 6 */
  0x00,  //...... 
  0x00,  //...... 
  0x00,  //...... 
  0x3c,  //..%%%% 
  0x08,  //....%. 
  0x08,  //....%. 
  0x08,  //....%. 
  0x08,  //....%. 
  0x08,  //....%. 
  0x08,  //....%. 
  0x88,  //%...%. 
  0x70,  //.%%%.. 
  0x00,  //...... 
  0x00,  //...... 
  0x00,  //...... 
  0x00,  //...... 


  /*Unicode: U+004b (K) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x90,  //%..%. 
  0xa0,  //%.%.. 
  0xc0,  //%%... 
  0xa0,  //%.%.. 
  0x90,  //%..%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+004c (L) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+004d (M) , Width: 7 */
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x82,  //%.....% 
  0x82,  //%.....% 
  0xc6,  //%%...%% 
  0xaa,  //%.%.%.% 
  0x92,  //%..%..% 
  0x82,  //%.....% 
  0x82,  //%.....% 
  0x82,  //%.....% 
  0x82,  //%.....% 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 


  /*Unicode: U+004e (N) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xc8,  //%%..% 
  0xa8,  //%.%.% 
  0x98,  //%..%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+004f (O) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0050 (P) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf0,  //%%%%. 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0051 (Q) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xa8,  //%.%.% 
  0x90,  //%..%. 
  0x68,  //.%%.% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0052 (R) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf0,  //%%%%. 
  0xa0,  //%.%.. 
  0x90,  //%..%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0053 (S) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x80,  //%.... 
  0x80,  //%.... 
  0x70,  //.%%%. 
  0x08,  //....% 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0054 (T) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0055 (U) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0056 (V) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x50,  //.%.%. 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0057 (W) , Width: 7 */
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x82,  //%.....% 
  0x82,  //%.....% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x6c,  //.%%.%%. 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 


  /*Unicode: U+0058 (X) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x50,  //.%.%. 
  0x20,  //..%.. 
  0x50,  //.%.%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0059 (Y) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x50,  //.%.%. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+005a (Z) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x08,  //....% 
  0x08,  //....% 
  0x10,  //...%. 
  0x20,  //..%.. 
  0x40,  //.%... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+005b ([) , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0xe0,  //%%% 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0xe0,  //%%% 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+005c (\) , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x80,  //%.. 
  0x80,  //%.. 
  0x80,  //%.. 
  0x40,  //.%. 
  0x40,  //.%. 
  0x40,  //.%. 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+005d (]) , Width: 3 */
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0xe0,  //%%% 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0x20,  //..% 
  0xe0,  //%%% 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 
  0x00,  //... 


  /*Unicode: U+005e (^) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x20,  //..%.. 
  0x50,  //.%.%. 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+005f (_) , Width: 8 */
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0x00,  //........ 
  0xff,  //%%%%%%%% 
  0x00,  //........ 
  0x00,  //........ 


  /*Unicode: U+0060 (`) , Width: 2 */
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x80,  //%. 
  0x40,  //.% 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 
  0x00,  //.. 


  /*Unicode: U+0061 (a) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x08,  //....% 
  0x78,  //.%%%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0062 (b) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf0,  //%%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0063 (c) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0064 (d) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x08,  //....% 
  0x08,  //....% 
  0x78,  //.%%%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0065 (e) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf8,  //%%%%% 
  0x80,  //%.... 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0066 (f) , Width: 4 */
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x30,  //..%% 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0xf0,  //%%%% 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 


  /*Unicode: U+0067 (g) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x78,  //.%%%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0068 (h) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x80,  //%.... 
  0x80,  //%.... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0069 (i) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x20,  //..%.. 
  0x00,  //..... 
  0xe0,  //%%%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+006a (j) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x08,  //....% 
  0x00,  //..... 
  0x38,  //..%%% 
  0x08,  //....% 
  0x08,  //....% 
  0x08,  //....% 
  0x08,  //....% 
  0x08,  //....% 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+006b (k) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x88,  //%...% 
  0x90,  //%..%. 
  0xa0,  //%.%.. 
  0xc0,  //%%... 
  0xa0,  //%.%.. 
  0x90,  //%..%. 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+006c (l) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xe0,  //%%%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0x20,  //..%.. 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+006d (m) , Width: 7 */
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0xec,  //%%%.%%. 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x82,  //%.....% 
  0x82,  //%.....% 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 


  /*Unicode: U+006e (n) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+006f (o) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0070 (p) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf0,  //%%%%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0xf0,  //%%%%. 
  0x80,  //%.... 
  0x80,  //%.... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0071 (q) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x78,  //.%%%% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x08,  //....% 
  0x08,  //....% 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0072 (r) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x98,  //%..%% 
  0xa0,  //%.%.. 
  0xc0,  //%%... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x80,  //%.... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0073 (s) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x70,  //.%%%. 
  0x88,  //%...% 
  0x80,  //%.... 
  0x70,  //.%%%. 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0074 (t) , Width: 4 */
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x40,  //.%.. 
  0xf0,  //%%%% 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x30,  //..%% 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 


  /*Unicode: U+0075 (u) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0076 (v) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x50,  //.%.%. 
  0x20,  //..%.. 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0077 (w) , Width: 7 */
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x82,  //%.....% 
  0x82,  //%.....% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x92,  //%..%..% 
  0x6c,  //.%%.%%. 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 
  0x00,  //....... 


  /*Unicode: U+0078 (x) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x50,  //.%.%. 
  0x20,  //..%.. 
  0x50,  //.%.%. 
  0x88,  //%...% 
  0x88,  //%...% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+0079 (y) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x88,  //%...% 
  0x78,  //.%%%% 
  0x08,  //....% 
  0x88,  //%...% 
  0x70,  //.%%%. 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+007a (z) , Width: 5 */
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0xf8,  //%%%%% 
  0x08,  //....% 
  0x10,  //...%. 
  0x20,  //..%.. 
  0x40,  //.%... 
  0x80,  //%.... 
  0xf8,  //%%%%% 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 
  0x00,  //..... 


  /*Unicode: U+007b ({) , Width: 4 */
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x30,  //..%% 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x80,  //%... 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x40,  //.%.. 
  0x30,  //..%% 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 
  0x00,  //.... 


  /*Unicode: U+007c (|) , Width: 1 */
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x80,  //% 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 
  0x00,  //. 


};


/*Store the glyph descriptions*/
static const uint8_t widths[] = {
    1, 3, 6, 5, 7, 5, 1, 3, 3, 5, 5, 2, 4, 1, 3, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 1, 2, 3, 4, 3, 5, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 7, 5,
    5, 5, 5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 3, 3, 3, 5, 8, 2, 5, 5, 5, 5, 5,
    4, 5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 4, 5, 5, 7, 5, 5, 5, 4, 1,
};

static constexpr const char* order = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

FontCharacter Fonts::getPixelOperatorBitmap(char character) {

  FontCharacter fc;
  fc.character = character;
  fc.maxWidth = 8;
  fc.maxHeight = 16;

  int offset = 0;
  for (int i=0; order[i] != '\0'; i++) {
    if (order[i] == character) {
      fc.charWidth = widths[i];
      for (int i=0; i<16; i++) {
        fc.data[i] = bitmap[offset + i];
      }
      return fc;
    }
    offset += 16;
  };

  __builtin_unreachable();
}

