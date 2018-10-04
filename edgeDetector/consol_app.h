#ifndef CONSOL_APP_H
#define CONSOL_APP_H
#include "filterfactory.h"
#include <QString>
#include <string>
class consol_app
{
public:
    consol_app(int threshold_value, int threshold_max_value);
    void run(const QString &input_directory,
             const std::string &output_directory);

private:
    int threshold_value, threshold_max_value;
    filterType type;
    double err, force;
};

#endif // CONSOL_APP_H
