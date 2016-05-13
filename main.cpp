#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <QVector2D>
#include <QVector3D>

#include <QList>
#include <QDebug>
int main(int argc, char *argv[])
{
    float double_radius=2*45;
    float tag_size=35;
    float max_circle=500;


    QVector2D p1_offset(-tag_size/2,tag_size/2);
    QVector2D p2_offset(tag_size/2,tag_size/2);
    QVector2D p3_offset(tag_size/2,-tag_size/2);
    QVector2D p4_offset(-tag_size/2,-tag_size/2);


    QList<QVector2D> directions;
    directions<<QVector2D(cos(M_PI/6)*double_radius,sin(M_PI/6)*double_radius)<<QVector2D(0,double_radius)
              <<QVector2D(-cos(M_PI/6)*double_radius,sin(M_PI/6)*double_radius)<<QVector2D(-cos(M_PI/6)*double_radius,-sin(M_PI/6)*double_radius)
              <<QVector2D(0,-double_radius)<<QVector2D(cos(M_PI/6)*double_radius,-sin(M_PI/6)*double_radius);

    std::vector<int> ids;
    std::vector< std::vector< cv::Point3f > > boardPoints;

    QList<QVector2D> visited;
    QList<QVector3D> to_visit;
    to_visit<<QVector3D(0,0,0);
    QVector3D curret_with_depth;
    QVector2D current;
    int tag_id=0;
    std::vector< cv::Point3f > tmp;
    cv::Mat debug_image(2*max_circle,2*max_circle,CV_8UC3);
    debug_image=cv::Scalar(255,255,255);
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
          tmp.clear();
          tmp.push_back(cv::Point3f(current.x()+p1_offset.x(),current.y()+p1_offset.y(),0));
          tmp.push_back(cv::Point3f(current.x()+p2_offset.x(),current.y()+p2_offset.y(),0));
          tmp.push_back(cv::Point3f(current.x()+p3_offset.x(),current.y()+p3_offset.y(),0));
          tmp.push_back(cv::Point3f(current.x()+p4_offset.x(),current.y()+p4_offset.y(),0));
          boardPoints.push_back(tmp);
          ids.push_back(tag_id);
          if(fmod(curret_with_depth.z(),2)==0)
              cv::putText(debug_image,std::to_string(tag_id),
                      cv::Point2f(current.x()+max_circle,-current.y()+max_circle),0,1,cv::Scalar(255,0,0));
          else
              cv::putText(debug_image,std::to_string(tag_id),
                      cv::Point2f(current.x()+max_circle,-current.y()+max_circle),0,1,cv::Scalar(0,255,0));

          tag_id++;
          visited.append(current);
          Q_FOREACH(QVector2D dir, directions){
              QVector3D tmp=current+dir;
              tmp.setZ(curret_with_depth.z()+1);
              to_visit.append(tmp);
          }
          qDebug()<<tag_id-1<<":"<<current;
    }
    cv::FileStorage fs("board.yml", cv::FileStorage::WRITE);
    fs<<"dictionary"<<11;
    fs<<"ids"<<ids;
    fs<<"points"<<boardPoints;
    fs.release();
    cv::imwrite("board_debug.png",debug_image);

}
