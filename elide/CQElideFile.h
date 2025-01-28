#include <QString>

class CQElideFile {
 public:
  CQElideFile(const QString &file);

  int width() const { return width_; }
  void setWidth(int i) { width_ = i; }

  const Qt::TextElideMode &elide() const { return elide_; }
  void setElide(const Qt::TextElideMode &e) { elide_ = e; }

  QString elide();

 private:
  bool tryElide(QString &res);

 private:
  QString file_;

  int               width_ { 0 };
  Qt::TextElideMode elide_ { Qt::ElideLeft };
};
