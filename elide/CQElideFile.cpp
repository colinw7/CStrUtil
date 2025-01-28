#include <CQElideFile.h>
#include <QStringList>
#include <set>

CQElideFile::
CQElideFile(const QString &file) :
 file_(file)
{
}

QString
CQElideFile::
elide()
{
  QString res;

  if (! tryElide(res))
    return "...";

  return res;
}

bool
CQElideFile::
tryElide(QString &res)
{
  if (elide_ == Qt::ElideNone || file_.length() < width_) {
    res = file_;
    return true;
  }

  auto strs = file_.split("/");

  int n = strs.length();
  if (n <= 1) return false;

  for (;;) {
    bool changed = false;

    if      (elide_ == Qt::ElideLeft) {
      // replace first path part with ... (if not already changed)
      int i = 0;

      while (i < n) {
        if (strs[i] != "...") {
          strs[i] = "...";
          changed = true;
          break;
        }

        ++i;
      }
    }
    else if (elide_ == Qt::ElideRight) {
      // replace last path part with ... (if not already changed)
      int i = n - 1;

      while (i >= 0) {
        if (strs[i] != "...") {
          strs[i] = "...";
          changed = true;
          break;
        }

        --i;
      }
    }
    else {
      // replace middle path part with ... (if not already changed)
      std::set<int> inds;

      int i1 = n/2;
      int i2 = i1 + 1;

      inds.insert(i1);
      if (i2 < n) inds.insert(i2);

      while (! inds.empty()) {
        for (auto i : inds) {
          if (strs[i] != "...") {
            strs[i] = "...";
            changed = true;
            break;
          }
        }

        if (changed)
          break;

        inds.clear();

        --i1;
        ++i2;

        if (i1 >= 0) inds.insert(i1);
        if (i2 <  n) inds.insert(i2);
      }
    }

    //---

    // remove duplicate ... parts
    QStringList strs1;

    int lastDot = -2;

    for (int i = 0; i < n; ++i) {
      if (strs[i] == "...") {
        bool dup = (lastDot == i - 1);

        lastDot = i;

        if (dup) {
          changed = true;
          continue;
        }
      }

      strs1.push_back(strs[i]);
    }

    if (! changed)
      break;

    //---

    // rebuild path and check if short enough
    strs = strs1;
    n    = strs.length();

    auto str1 = strs.join("/");

    if (str1.length() < width_) {
      res = str1;
      return true;
    }
  }

  return false;
}
