#ifndef CONSOL_APP_H
#define CONSOL_APP_H
#include <string>
#include <QString>
#include "filterfactory.h"
class consol_app
{
public:
    consol_app();
    void run(const QString& input_directory, const std::string& output_directory);
private:
    int threshold_value,threshold_max_value;
    filterType type;
    double err,force;
};

#endif // CONSOL_APP_H
