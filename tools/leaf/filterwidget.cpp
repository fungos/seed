#include "filterwidget.h"

#include "filterwidget.h"

#include <QIcon>
#include <QPixmap>
#include <QImage>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QToolButton>
#include <QWidgetAction>
#include <QDebug>

FilterWidget::FilterWidget(QWidget *parent)
	: QLineEdit(parent)
	, m_patternGroup(new QActionGroup(this))
{
	setClearButtonEnabled(true);
	connect(this, SIGNAL(textChanged(QString)), this, SIGNAL(filterChanged()));

	QMenu *menu = new QMenu(this);
	m_caseSensitivityAction = menu->addAction(tr("Case Sensitive"));
	m_caseSensitivityAction->setCheckable(true);
	connect(m_caseSensitivityAction, SIGNAL(toggled(bool)), this, SIGNAL(filterChanged()));

	menu->addSeparator();
	m_patternGroup->setExclusive(true);
	QAction *patternAction = menu->addAction("Fixed String");
	patternAction->setData(QVariant(int(QRegExp::FixedString)));
	patternAction->setCheckable(true);
	patternAction->setChecked(true);
	m_patternGroup->addAction(patternAction);
	patternAction = menu->addAction("Regular Expression");
	patternAction->setCheckable(true);
	patternAction->setData(QVariant(int(QRegExp::RegExp2)));
	m_patternGroup->addAction(patternAction);
	patternAction = menu->addAction("Wildcard");
	patternAction->setCheckable(true);
	patternAction->setData(QVariant(int(QRegExp::Wildcard)));
	m_patternGroup->addAction(patternAction);
	connect(m_patternGroup, SIGNAL(triggered(QAction*)), this, SIGNAL(filterChanged()));

	const QIcon icon = QIcon(QPixmap(":/images/find.png"));
	QToolButton *optionsButton = new QToolButton;
	optionsButton->setCursor(Qt::ArrowCursor);
	optionsButton->setFocusPolicy(Qt::NoFocus);
	optionsButton->setStyleSheet("* { border: none; }");
	optionsButton->setIcon(icon);
	optionsButton->setMenu(menu);
	optionsButton->setPopupMode(QToolButton::InstantPopup);

	QWidgetAction *optionsAction = new QWidgetAction(this);
	optionsAction->setDefaultWidget(optionsButton);
	addAction(optionsAction, QLineEdit::LeadingPosition);
}

Qt::CaseSensitivity FilterWidget::caseSensitivity() const
{
	return m_caseSensitivityAction->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

void FilterWidget::setCaseSensitivity(Qt::CaseSensitivity cs)
{
	m_caseSensitivityAction->setChecked(cs == Qt::CaseSensitive);
}

static inline QRegExp::PatternSyntax patternSyntaxFromAction(const QAction *a)
{
	return static_cast<QRegExp::PatternSyntax>(a->data().toInt());
}

QRegExp::PatternSyntax FilterWidget::patternSyntax() const
{
	return patternSyntaxFromAction(m_patternGroup->checkedAction());
}

void FilterWidget::setPatternSyntax(QRegExp::PatternSyntax s)
{
	foreach (QAction *a, m_patternGroup->actions())
	{
		if (patternSyntaxFromAction(a) == s)
		{
			a->setChecked(true);
			break;
		}
	}
}
