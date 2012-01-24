#ifndef __GSM_IBITMAP_HPP
#define __GSM_IBITMAP_HPP

namespace gsm {

    class IBitmap {
    public:
        virtual ~IBitmap() {}
        virtual unsigned width() const = 0;
        virtual unsigned height() const = 0;
        virtual const void * data() const = 0;
    };

} // ns gsm

#endif // __GSM_IBITMAP_HPP
