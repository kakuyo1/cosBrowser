#ifndef COMBOLINE_H
#define COMBOLINE_H

#include <QLineEdit>

class ComboLine : public QLineEdit
{
    Q_OBJECT
public:
    ComboLine(QWidget* parent = nullptr);
    ComboLine(const QStringList& items, QWidget* parent = nullptr);
    ~ComboLine();

    // 设置下拉表
    void setItems(const QStringList& items);
    void setItems(const QList<int> items);
    void setItems(int start = 1, int len = 10);

    // 获取下拉列表
    QStringList items() const;

signals:
    void itemSelected(const QString& itemText);

protected:
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // COMBOLINE_H
