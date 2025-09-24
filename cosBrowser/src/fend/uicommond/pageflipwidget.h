#ifndef PAGEFLIPWIDGET_H
#define PAGEFLIPWIDGET_H

#include <QWidget>

namespace Ui {
class PageFlipWidget;
}

class PageFlipWidget  : public QWidget
{
    Q_OBJECT

public:
    explicit PageFlipWidget(QWidget *parent = nullptr);
    ~PageFlipWidget();

    // 总条数
    int totalRow() const;
    // 单页最大行数
    int maxRowPerPage() const;
    // 总页数
    int pageCount() const;
    // 首页
    int isFirstPage() const;
    // 尾页
    int isLastPage() const;

    // 设置总条数
    void setTotalRow(int newTotalRow);
    // 设置单页最大行数
    void setMaxRowPerPage(int newMaxRowPerPage);
    // 设置单页显示行数下拉列表
    void setRowPerPageItems(const QList<int> &items);

signals:
    // start: 起始行号, maxLength: 每页最大行数
    void pageNumberChanged(int start, int maxLength);

public slots:
    // 下一页
    void nextPage();
    // 上一页
    void previousPage();
    // 跳转页面
    void jumpToPage(int page);

private slots:
    // 选择页码
    void onCurrentPageSelected(const QString& text);
    // 选择单页显示最大行数
    void onMaxRowPerPageSelected(const QString& text);

private:
    // 设置行数列表显示
    void setPageCount();
    // 重置为首页
    void reset();
    // 切换页面
    void switchPage(int newCurrentPage);
private:
    Ui::PageFlipWidget *ui;
    int m_totalRow;            // 总条数
    int m_maxRowPerPage;      // 单页最大行数
    int m_currentPage;        // 当前页码
};

#endif // PAGEFLIPWIDGET_H
