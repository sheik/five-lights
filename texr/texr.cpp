/* M_PI is defined in math.h in the case of Microsoft Visual C++, and
 * Solaris needs math.h for M_PI and floor()
 */
#if defined(_MSC_VER)
#define _USE_MATH_DEFINES
#endif 

#include <string>
#include <iostream>
#include <cairommconfig.h>
#include <cairomm/cairomm.h>

#include <cmath>


const int WIDTH = 640;
const int HEIGHT = 480;
const int NUM_TEXT = 20;
const int TEXT_SIZE = 22;

/* Draw the word cairo at NUM_TEXT different angles */
void draw(Cairo::RefPtr<Cairo::Context> cr, int width, int height)
{
    int i, x_off, y_off;
    Cairo::TextExtents extents;
    std::string text("cairo");

    cr->select_font_face("Bitstream Vera Sans", Cairo::FONT_SLANT_NORMAL,
            Cairo::FONT_WEIGHT_NORMAL);
    cr->set_font_size(TEXT_SIZE);

    Cairo::FontOptions font_options;

    font_options.set_hint_style(Cairo::HINT_STYLE_NONE);
    font_options.set_hint_metrics(Cairo::HINT_METRICS_OFF);
    font_options.set_antialias(Cairo::ANTIALIAS_GRAY);

    cr->set_font_options(font_options);

    cr->set_source_rgb(0.0, 0.0, 0.0);

    cr->translate(width / 2.0, height / 2.0);

    cr->get_text_extents(text, extents);

    if (NUM_TEXT == 1)
    {
        x_off = y_off = 0;
    }
    else
    {
        y_off = (int) - floor(0.5 + extents.height / 2.0);
        x_off = (int) floor(0.5 + (extents.height + 1.0) / (2.0 * tan (M_PI / NUM_TEXT)));
    }

    for (i=0; i < NUM_TEXT; i++)
    {
        cr->save();
        cr->rotate(2 * M_PI * i / NUM_TEXT);
        cr->set_line_width(1.0);
        cr->set_source_rgb(1, 0, 0);
        cr->stroke();
        cr->move_to(x_off - extents.x_bearing, y_off - extents.y_bearing);
        cr->set_source_rgb(0, 0, 0);
        cr->show_text("Five Lights!");
        cr->restore();
    }
}

int main (void)
{
    Cairo::RefPtr<Cairo::ImageSurface> surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, WIDTH, HEIGHT);
    Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(surface);
    draw(cr, WIDTH, HEIGHT);
#ifdef CAIRO_HAS_PNG_FUNCTIONS
    const char* filename = "text-rotate.png";
    surface->write_to_png(filename);
    std::cout << "Wrote file " << filename << std::endl;
#else
    std::cout << "You must compile cairo with PNG support for this example to work" << std::endl;
#endif
}
