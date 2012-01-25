#include <Windows.h>
#include "../../ibitmap.hpp"
#include "mswinerr.hpp"
#include "mswinfont.hpp"

namespace gsm {

//--- PRIVATE TYPES ----------------------------------------------------------

class GDIBitmap: public IBitmap {
public:
    GDIBitmap(unsigned w, unsigned h, unsigned bits) {
        BITMAPINFOHEADER bih;
        initInfoHeader(bih, sizeof(BITMAPINFOHEADER), w, h, bits);
        // Create the bitmap
        hbmp = CreateDIBSection(NULL, (const BITMAPINFO*) &bih, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        if (hbmp == 0) throw EMSWinError(GetLastError(), "CreateDIBSection (GDIBitmap)");
    }

    virtual ~GDIBitmap() {
        DeleteObject((HGDIOBJ)hbmp); }

    virtual unsigned width() const {
        BITMAP bm;
        GetObject(hbmp, sizeof(bm), &bm);
        return (unsigned) bm.bmWidth; }

    virtual unsigned height() const {
        BITMAP bm;
        GetObject(hbmp, sizeof(bm), &bm);
        return (unsigned) bm.bmHeight; }

    virtual const void * data() const { 
        return pixels; 
    }

    HBITMAP handle() const { return hbmp; }

protected:

    GDIBitmap(): hbmp(0), pixels(NULL) {}

    typedef struct
    {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD bmiColors[256];
    } BITMAPINFO_8bpp;

    static void initInfoHeader(BITMAPINFOHEADER &bih, size_t size, unsigned w, unsigned h, unsigned bits) {
        // Bitmap parameters
        bih.biSize = size;
        bih.biWidth = w, bih.biHeight = h;
        bih.biPlanes = 1;
        bih.biBitCount = bits;
        bih.biCompression = BI_RGB;
        bih.biSizeImage = 0; // auto
        bih.biXPelsPerMeter = 96 * 254;
        bih.biYPelsPerMeter = 96 * 254;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;
    }

    static void initGrayscalePalette(BITMAPINFO_8bpp &bif)
    {
        bif.bmiHeader.biClrUsed = 256;
        bif.bmiHeader.biClrImportant = 256;
        for (unsigned i = 0; i < 256; i++) { 
            RGBQUAD & rgb = bif.bmiColors[i];
            rgb.rgbRed = i, rgb.rgbGreen = i, rgb.rgbBlue = i;
        }
    }

    byte_t *pixels;
    HBITMAP hbmp;
};

#ifdef NOT_DEFINED

class GrayscaleBitmap: public GDIBitmap {
public:
    GrayscaleBitmap(unsigned w, unsigned h) {
        BITMAPINFO_8bpp bmpinfo;
        initInfoHeader(bmpinfo.bmiHeader, sizeof(BITMAPINFO_8bpp), w, h, 8);
        initGrayscalePalette(bmpinfo);
        // Create the bitmap
        hbmp = CreateDIBSection(NULL, (const BITMAPINFO*) &bmpinfo, DIB_RGB_COLORS, (void**)&pixels, NULL, 0);
        if (hbmp == 0) throw EMSWinError(GetLastError(), "CreateDIBSection (GrayscaleBitmap)");
    }

    static GrayscaleBitmap *fromRGB(GDIBitmap &src) {
        /*
        typedef byte_t rgb_pixel[4];
        typedef byte_t gray_pixel;
        rgb_pixel *srcbits;
        gray_pixel *destbits;
        GetBitmapBits(src.handle(), &srcbits);
        GetBitmapBits(hbmp, &destbits);
        */

        /*
        BITMAPINFO_8bpp bmpinfo;
        initInfoHeader(bmpinfo.bmiHeader, sizeof(BITMAPINFO_8bpp), src.width(), src.height(), 8);
        //initGrayscalePalette(bmpinfo);
        HDC hDC = CreateCompatibleDC(NULL);
        SelectObject(hDC, src.handle());

        GrayscaleBitmap *graybmp = new GrayscaleBitmap(src.width(), src.height());
        CHECK(GetDIBits, (hDC, src.handle(), 0, src.height(), src.buffer(), (LPBITMAPINFO) &bmpinfo, DIB_RGB_COLORS));

        SelectObject(hDC, graybmp->handle());
        CHECK(SetDIBits, (hDC, graybmp->handle(), 0, src.height(), graybmp->pixels, (LPBITMAPINFO) &bmpinfo, DIB_RGB_COLORS));

        CHECK(DeleteDC, (hDC));
        */

        /*
        GrayscaleBitmap *graybmp = new GrayscaleBitmap(src.width(), src.height());
        HDC hSrc = CreateCompatibleDC(NULL);
        HGDIOBJ hPrevSrc = SelectObject(hSrc, src.handle());
        HDC hDst = CreateCompatibleDC(NULL);
        HGDIOBJ hPrevDst = SelectObject(hDst, graybmp->handle());

        unsigned w = src.width(), h = src.height();
        for (unsigned y = 0; y < h; y ++) {
            for (unsigned x = 0; x < w; x ++) {
                COLORREF clr = GetPixel(hSrc, x, y);
                SetPixel(hDst, x, y, clr);
            }
        }

        DeleteDC(hSrc);
        DeleteDC(hDst);
        */

        //BITMAP srcbmp;
        //GetObject(src.handle(), sizeof(BITMAP), &srcbmp);

        GrayscaleBitmap *graybmp = new GrayscaleBitmap(src.width(), src.height());

        typedef byte_t color_t[3];
        const color_t *src_pixels = (const color_t*) src.data(); //srcbmp.bmBits;
        byte_t *dst_pixels = (byte_t*) graybmp->buffer(); //srcbmp.bmBits;

        unsigned w = src.width(), h = src.height();
        for (unsigned y = 0; y < h; y ++) {
            for (unsigned x = 0; x < w; x ++) {
                unsigned i = y * w + x;
                byte_t gray = (byte_t) ((unsigned) src_pixels[i][0] + (unsigned) src_pixels[i][1] + (unsigned) src_pixels[i][2]) / 3;
                dst_pixels[i] = gray;
            }
        }


        return graybmp;
    }
};

#endif

class GrayscaleBitmap: public IBitmap {
public:
    GrayscaleBitmap(const GDIBitmap &src) {
        w = src.width();
        h = src.height();
        typedef byte_t color_t[3];
        const color_t *src_pixels = (const color_t*) src.data();
        pixels = new byte_t[w*h];
        unsigned w = src.width(), h = src.height();
        bool nonempty = false;
        for (unsigned y = 0; y < h; y ++) {
            for (unsigned x = 0; x < w; x ++) {
                unsigned i = y * w + x;
                const color_t &clr = src_pixels[i];
                byte_t gray = (byte_t) (((unsigned) clr[0] + (unsigned) clr[1] + (unsigned) clr[2]) / 3);
                pixels[i] = gray;
                if (gray != 0) nonempty = true;
            }
        }
    }
    virtual ~GrayscaleBitmap() { delete[] pixels;}
    virtual unsigned width() const { return w; }
    virtual unsigned height() const { return h; }
    virtual const void * data() const { return pixels; }

private:
    unsigned w, h;
    byte_t *pixels;
};

//--- PRIVATE ROUTINES --------------------------------------------------------

static void
getGlyphMetricsTT(HDC hdc, const TEXTMETRIC &tm, unicode_t ch, IFont::GlyphMetrics & metrics) 
{
    ABC abc;
    GetCharABCWidthsW(hdc, ch, ch, &abc);
    metrics.xMin = abc.abcA - 1;
    metrics.xMax = abc.abcA + abc.abcB + 1;
    //metrics.bbox.xMax += abs(abc.abcC);
    metrics.yMin = - tm.tmAscent - 1;
    metrics.yMax = tm.tmDescent + 1;
    metrics.adv_w = abc.abcA + abc.abcB + abc.abcC;
    metrics.adv_h = 0; //proxy->metrics.tmAscent + proxy->metrics.tmDescent;
}

static void
getGlyphMetricsNonTT(HDC hdc, const TEXTMETRIC &tm, unicode_t ch, IFont::GlyphMetrics & metrics) 
{
    INT w;
    GetCharWidth32(hdc, ch, ch, &w);
    metrics.xMin = 0 - 1;
    metrics.xMax = w + 1;
    metrics.yMin = - tm.tmAscent -1;
    metrics.yMax = tm.tmDescent +1;
    metrics.adv_w = w;
    metrics.adv_h = 0; //proxy->metrics.tmAscent + proxy->metrics.tmDescent;
}

static unsigned
next_pow2(unsigned a)
{
    unsigned rval;
    for (rval = 1; rval < a; rval *= 2);
    return rval;
}

//--- MSWINFONT IMPLEMENTATION ------------------------------------------------

MSWinFont::MSWinFont(MSWinFontProvider *prov, HGDIOBJ hfont_)
    : provider(prov), hfont(hfont_)
{
    // Create a dummy bitmap (1x1 pixels) and a DC for it
    hbmp = CreateBitmap(1, 1, 1, 24, NULL);
    hdc = CreateCompatibleDC(NULL);
    HGDIOBJ hPrevBmp = SelectObject(hdc, hbmp); // discarding previous bitmap (if any)
    SelectObject(hdc, hfont);
    SetMapMode(hdc, MM_TEXT);
    SetTextColor(hdc, RGB(255, 255, 255) );
    SetBkMode(hdc, TRANSPARENT);
    //SetBkColor(hdc, RGB(100,100,100) );
    //SetBkColor(hdc, RGB(0,0,0) );
    SetTextAlign(hdc, TA_BASELINE);

    // Get the font metrics
    if (!GetTextMetrics(hdc, &metrics))
        throw EMSWinError(GetLastError(), "GetTextMetrics");

    // Get the Character Set supported by this font
    DWORD size = GetFontUnicodeRanges(hdc, NULL);
    LPGLYPHSET glyphset = (LPGLYPHSET) new char[size];
    if (GetFontUnicodeRanges(hdc, glyphset) == 0)
        throw EMSWinError(GetLastError(), "GetFontUnicodeRanges");
    for (unsigned i = 0; i < glyphset->cRanges; i ++) {
        WCRANGE & src = glyphset->ranges[i];
        charset.add(src.wcLow, src.cGlyphs);
    }
}

MSWinFont::~MSWinFont()
{
    DeleteDC(hdc);
    DeleteObject(hbmp);
}

const MSWinFont::Rasterization
MSWinFont::rasterize(const CharacterSet & set, CharacterSet::iterator & it, unsigned max_edge, const RasterizeOptions opts)
{
    max_edge = max_edge > 0 ? max_edge : DEFAULT_MAX_BITMAP_SIZE;

    // Find out how many character glyphs will fit onto a single bitmap
    bool tt = (metrics.tmPitchAndFamily & TMPF_TRUETYPE) != 0;
    unsigned w = 0, wmax = 0;
    unsigned h = 0, hrmax = 0;
    GlyphMetrics gm;
    CharacterSet::iterator itm = it;
    for (; itm != set.end(); itm ++) 
    {
        unicode_t ch = *itm;
        // Is there a glyph for the character ?
        if (charset.find(ch) != charset.end())
        {
            // Get the glyph's metrics
            if (tt) getGlyphMetricsTT(hdc, metrics, ch, gm); else getGlyphMetricsNonTT(hdc, metrics, ch, gm);
            // Doesn't fit on the current row?
            if ((w + gm.width()) > max_edge) {
                // Begin a new row
                if (w > wmax) wmax = w;
                w = 0;
                h += hrmax;
                hrmax = 0;
            }
            // Glyph does not fit vertically ? 
            if ((h + gm.height()) > max_edge) {
                // Stop here (finishing off the current row)
                if (w > wmax) wmax = w;
                break;
            }
            // Adapt row height if too low
            if (gm.height() > hrmax) hrmax = gm.height();
            w += gm.width();
        }
    }
    if (w > wmax) wmax = w;
    h += hrmax;

    // Allocate and select the bitmap
    w = next_pow2(wmax);
    h = next_pow2(h);
    assert(w <= max_edge && h <= max_edge);
    GDIBitmap *bitmap = new GDIBitmap(w, h, 24);
    HGDIOBJ holdbmp = SelectObject(hdc, bitmap->handle() );

    // Initialize the Rasterization object
    Rasterization rast;

    // Now render the glyphs into the bitmap
    unsigned x = 0, y = 0;
    hrmax = 0;
    for (CharacterSet::iterator itr = it; itr != itm; itr ++)
    {
        unicode_t ch = *itr;
        // Is there a glyph for the character ?
        if (charset.find(ch) != charset.end()) {
            // Get the glyph's metrics
            if (tt) getGlyphMetricsTT(hdc, metrics, ch, gm); else getGlyphMetricsNonTT(hdc, metrics, ch, gm);
            // Doesn't fit on the current row?
            if ((x + gm.width()) > w) {
                // Begin a new row
                x = 0;
                y += hrmax;
                hrmax = 0;
            }
            // Render the glyph at the appropriate spot
            if (! TextOutW(hdc, x - gm.xMin, y - gm.yMin, &ch, 1) )
                throw EMSWinError(GetLastError(), "TextOutW");
            // Make sure row will be high enough for highest glyph in it
            if (gm.height() > hrmax) hrmax = gm.height();
            // Store Glyph Box info
            GlyphBox gb;
            static_cast<GlyphMetrics&>(gb) = gm;
            gb.xLeft = x;
            gb.yTop = y;
            rast.glyph_boxes.push_back(gb);
            // Add Character to set
            rast.character_set.add(ch);
            // Advance to next spot
            x += gm.width();
        }
    }

    // Convert the bitmap to grayscale and discard the GDI bitmap
    rast.bitmap = new GrayscaleBitmap(*bitmap);
    delete bitmap;

    // Cleanup
    SelectObject(hdc, holdbmp);

    it = itm;
    return rast;
}

const Extents
MSWinFont::getTextExtents(const unicode_t *text, unsigned len) 
{
    RECT rect = { 0, 0, LONG_MAX, LONG_MAX };

    DrawTextW(hdc, text, (int) len, &rect, DT_CALCRECT | DT_NOPREFIX | DT_WORDBREAK);

    Extents ext;
    ext.w = (unsigned) (rect.right - rect.left);
    ext.h = (unsigned) (rect.bottom - rect.top);

    return ext;
}

} // ns gsm
