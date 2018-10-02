#include "consol_app.h"
#include <QDirIterator>
#include <tbb/parallel_for.h>
#include "processing.h"

consol_app::consol_app()
{

}

void consol_app::run()
{
    QDirIterator it(input_directory, QDirIterator::NoIteratorFlags);
    std::vector<QString> sources;
    auto func = [] (QString str,QString delimiter)
       {
        auto tmpList=str.split(delimiter);
        return tmpList.value(tmpList.length()-1);
    };
    while (it.hasNext())
    {
        QString filename=it.next();
        auto tmp=func(filename,"/");
        auto ext=func(tmp,".");
        if(ext.toStdString()=="jpg" || ext.toStdString()=="png")
        {
            sources.push_back(filename);
        }
    }
  ///-----------------------run filter ---------------------------
  tbb::parallel_for(size_t(0), sources.size(), size_t(1),
                  [=](size_t i)
                  {
                      QString filename=sources[i];
                      auto tmp=func(filename,"/");
                      std::cout<<"Procesing "<< filename.toStdString() <<std::endl;
                      //----------------- procesing ---------------------------

                      cv::Mat image = cv::imread(filename.toStdString(),CV_LOAD_IMAGE_COLOR);
                      cv::Mat dest=procesing(image,threshold_value,threshold_max_value,type,err,force,true);
                      //---------------write result -----------------------------------
                      std::cout<<"Writing "<< output_directory+tmp.toStdString() <<std::endl;
                      std::string output_name=output_directory+tmp.toStdString();
                      cv::imwrite(output_name, dest);
                  });
}
