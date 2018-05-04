#ifndef _LCD_H_
#define _LCD_H_

#define LCD_DC (1 << 6)
#define LCD_CE (1 << 4)
#define LCD_RST (1 << 5)

#define SPI_MOSI (1 << 5)
#define SPI_CLK (1 << 7)

extern const struct CHINESE_ASCII chinese[];

extern const unsigned char AVR_bmp[];

extern void lcd_init(void);

extern void lcd_clear(unsigned char data);

extern void lcd_set_position(unsigned char begin_row, unsigned char begin_clo);

extern void lcd_display_byte(unsigned char data, unsigned char command);

extern void lcd_display_ascii_char(unsigned char c);

extern void lcd_display_english_string(unsigned char begin_row, unsigned char begin_col,
                                       char *str);

extern void lcd_display_chinese_char(unsigned char row, unsigned char col,
                                     unsigned char *str);

extern void lcd_draw_bmp_pixel(unsigned char begin_row,
                               unsigned char begin_col,
                               unsigned char *map,
                               unsigned char pix_long, unsigned char pix_hight);
#endif /* _LCD_H_ */