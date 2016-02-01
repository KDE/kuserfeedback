#ifndef USERFEEDBACK_ANALYZER_SURVEYDIALOG_H
#define USERFEEDBACK_ANALYZER_SURVEYDIALOG_H

#include <QDialog>

#include <memory>

class QUrl;

namespace UserFeedback {
namespace Analyzer {

namespace Ui
{
class SurveyDialog;
}

class Survey;

class SurveyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SurveyDialog(QWidget *parent = nullptr);
    ~SurveyDialog();

    Survey survey() const;
    void setSurvey(const Survey &survey);

private:
    std::unique_ptr<Ui::SurveyDialog> ui;
};

}
}

#endif // USERFEEDBACK_ANALYZER_SURVEYDIALOG_H
