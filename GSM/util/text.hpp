#ifndef __GSM_UTIL_TEXT_HPP
#define __GSM_UTIL_TEXT_HPP

#include "bitset.hpp"
#include "../ifont.hpp"

namespace gsm {

/** Text Traversal callback function prototype. The callback function will be called for 
    each character in the span being traversed.
    Parameters:
    userdata        pointer to user data, specified when calling traverseText()
    char_idx        character index
    bbox            Bounding Box of the current character.
    x, y            Insert position of the current character.
    dx, dy          Advancement required by the current character.
 */
typedef void (*ttCallbackFunc)(void *userdata, unsigned char_idx, const BoundingBox &bbox, int x, int y, int dx, int dy);

/** Traverse text.
 */
void traverseText(IFont *font, const unicode_t *text, size_t len, int linespacing, ttCallbackFunc cb, void *userdata);

/** Get the bounding box (relative to an assumed insertion point at 0,0) of a span of text.
 */
const BoundingBox getTextBounds(IFont *font, const unicode_t *text, size_t len = 0, int linespacing = 0);

} // ns gsm

#endif // __GSM_UTIL_TEXT_HPP
