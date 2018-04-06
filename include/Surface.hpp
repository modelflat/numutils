#ifndef NUMUTILS_SURFACE_HPP
#define NUMUTILS_SURFACE_HPP

#include <vector>

namespace nya {

template< typename T, bool rowMajor = true, typename Alloc = std::allocator<T> >
class Surface : public std::vector<T, Alloc> {

    using vector = std::vector<T, Alloc>;
    using size_type = typename vector::size_type;
    using allocator_type = typename vector::allocator_type;

protected:

    size_type xSize;
    size_type ySize;

public:
    Surface() : vector() {
    }

    explicit Surface(const allocator_type& __a)
        : vector(__a) {
    }

    Surface(size_type xSize, size_type ySize)
        : vector(xSize * ySize), xSize(xSize), ySize(ySize) {
    }

    Surface(size_type xSize, size_type ySize, const allocator_type& __a)
        : vector(xSize * ySize, __a), xSize(xSize), ySize(ySize) {
    }

    Surface(size_type xSize, size_type ySize, const T& __value, const allocator_type& __a = allocator_type())
        : vector(xSize * ySize, __value, __a), xSize(xSize), ySize(ySize) {
    }

    Surface(const Surface& __x)
        : vector(__x), xSize(__x.xSize), ySize(__x.ySize) {
    }

    Surface(Surface&& __x) noexcept
        : vector(std::forward<Surface>(__x)), xSize(__x.xSize), ySize(__x.ySize) {
    }

    Surface(const Surface& __x, const allocator_type& __a)
        : vector(__x, __a), xSize(__x.xSize), ySize(__x.ySize) {
    }

    Surface(Surface&& __x, const allocator_type& __a)
        : vector(std::forward<Surface>(__x), __a), xSize(__x.xSize), ySize(__x.ySize) {
    }

    inline Surface& operator=(const Surface& __x) noexcept {
        this->xSize = __x.xSize;
        this->ySize = __x.ySize;
        return vector::operator=(__x);
    }

    inline Surface& operator=(Surface&& __x) noexcept {
        this->xSize = __x.xSize;
        this->ySize = __x.ySize;
        return vector::operator=(std::forward<Surface>(__x));
    }

    /**
     * @brief NOTE: unlike std::vector::at(), this function does no range checking!
     */
    inline T& at(size_type x, size_type y) noexcept {
        if constexpr (rowMajor) {
            return *(vector::begin() + x * ySize + y);
        } else {
            return *(vector::begin() + y * xSize + x);
        }
    }

    /**
     * @brief NOTE: unlike std::vector::at(), this function does no range checking!
     */
    inline const T& at(size_type x, size_type y) const noexcept {
        if constexpr (rowMajor) {
            return *(vector::begin() + x * ySize + y);
        } else {
            return *(vector::begin() + y * xSize + x);
        }
    }

    /**
     * @brief Returns size of x dimension
     */
    inline size_type sizeX() const noexcept {
        return this->xSize;
    }

    inline size_type rowCount() const noexcept {
        if constexpr (rowMajor) {
            return xSize;
        } else {
            return ySize;
        }
    }

    /**
     * @brief Returns size of y dimension
     */
    inline size_type sizeY() const noexcept {
        return this->ySize;
    }

    inline size_type columnCount() const noexcept {
        if constexpr (rowMajor) {
            return ySize;
        } else {
            return xSize;
        }
    }

};

} // nya

#endif //NUMUTILS_SURFACE_HPP
