#ifndef ORWELL_H
#define ORWELL_H

#include <QMainWindow>

#include <memory>

namespace Ui { class Orwell; }

class Orwell : public QMainWindow
{
public:
    explicit Orwell(QWidget *parent = nullptr);
    ~Orwell();

private:
    void loadStats();
    void writeStats();

    std::unique_ptr<Ui::Orwell> ui;
};

#endif
