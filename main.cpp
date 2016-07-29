#include <QVector2D>
#include <QVector3D>
#include <QImage>
#include <QList>
#include <QPainter>
#include <iostream>
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMatrix3x3>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QMatrix3x3 rotationMatrix;
    rotationMatrix(0,0)=1;
    rotationMatrix(0,1)=0;
    rotationMatrix(0,2)=0;

    rotationMatrix(1,0)=0;
    rotationMatrix(1,1)=cos(0.14310645724258);
    rotationMatrix(1,2)=-sin(0.14310645724258);

    rotationMatrix(2,0)=0;
    rotationMatrix(2,1)=sin(0.14310645724258);
    rotationMatrix(2,2)=cos(0.14310645724258);

    float double_radius=2*45;
    float tag_size=35;
    float max_circle=500;

    QList<QVector2D> directions;
    directions<<QVector2D(cos(M_PI/6)*double_radius,sin(M_PI/6)*double_radius)<<QVector2D(0,double_radius)
              <<QVector2D(-cos(M_PI/6)*double_radius,sin(M_PI/6)*double_radius)<<QVector2D(-cos(M_PI/6)*double_radius,-sin(M_PI/6)*double_radius)
              <<QVector2D(0,-double_radius)<<QVector2D(cos(M_PI/6)*double_radius,-sin(M_PI/6)*double_radius);

    QList<int> ids;
    QList<QVector3D> boardPoints;

    QList<QVector2D> visited;
    QList<QVector3D> to_visit;

    to_visit<<QVector3D(0,0,0);

    QVector3D curret_with_depth;
    QVector2D current;

    int tag_id=0;

    QImage debug_image(2*max_circle,2*max_circle,QImage::Format_RGB32);
    debug_image.fill(QColor(255,255,255));
    while(!to_visit.isEmpty()){
          curret_with_depth=to_visit.first();
          to_visit.pop_front();
          current=curret_with_depth.toVector2D();

          if(current.length()>max_circle)
              continue;

          bool skip=false;

          Q_FOREACH(QVector2D prev_center, visited )
              if(prev_center.distanceToPoint(current)<1){
                  skip=true;
                  break;
              }

          if(skip)
              continue;

          boardPoints.push_back(QVector3D(current.x(),current.y(),0));

          ids.push_back(tag_id);
          QPainter paint;
          paint.begin(&debug_image);
          paint.setFont(QFont("Arial", 12));
          if(fmod(curret_with_depth.z(),2)==0)
              paint.setPen(QColor(255,0,0));
          else
              paint.setPen(QColor(0,255,0));
          QPoint point(current.x()+max_circle,-current.y()+max_circle);
          QString text=QString::number(tag_id);
          paint.drawText(point,text);
          paint.end();

          tag_id++;

          visited.append(current);

          Q_FOREACH(QVector2D dir, directions){
              QVector3D tmp=current+dir;
              tmp.setZ(curret_with_depth.z()+1);
              to_visit.append(tmp);
          }
    }
    QFile outputFile("board_configuration.dat");
    if(outputFile.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
        QTextStream stream(&outputFile);
        //N of tags
        stream<<tag_id<<"\n";
        stream<<"\n";

        for(int i=0;i<tag_id;i++){
        stream<<i<<"\n";
        stream<<tag_size<<"\n";
        stream<<rotationMatrix(0,0)<<" "<<rotationMatrix(0,1)<<" "<<rotationMatrix(0,2)<<" "<< boardPoints.at(i).x() << "\n";
        stream<<rotationMatrix(1,0)<<" "<<rotationMatrix(1,1)<<" "<<rotationMatrix(1,2)<<" "<< boardPoints.at(i).y() << "\n";
        stream<<rotationMatrix(2,0)<<" "<<rotationMatrix(2,1)<<" "<<rotationMatrix(2,2)<<" "<< boardPoints.at(i).z() << "\n";
        stream<<"\n";
        }
        stream.flush();
        outputFile.close();
    }
    if(!debug_image.save("board_debug.png"))
        qDebug()<<"Can't save image";

    return a.exec();

}
