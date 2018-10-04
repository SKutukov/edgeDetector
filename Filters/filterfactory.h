#ifndef FILTERFACTORY_H
#define FILTERFACTORY_H

#include "filter.h"
#include <memory>
/**
 * @brief The procType enum
 */
enum filterType { SOBEL, CANNY, LAPLAS, IRR_FILTER, CONV };

class FilterFactory
{
public:
    static std::shared_ptr<Filter> createFilter(filterType type);

private:
    FilterFactory();
};

#endif // FILTERFACTORY_H
