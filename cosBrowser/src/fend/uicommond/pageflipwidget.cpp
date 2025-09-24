#include "pageflipwidget.h"
#include "ui_pageflipwidget.h"

PageFlipWidget::PageFlipWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PageFlipWidget)
    , m_totalRow(0)
    , m_maxRowPerPage(10)
    , m_currentPage(1)
{
    ui->setupUi(this);
    // 初始化
    ui->lineEditMaxRows->setAlignment(Qt::AlignCenter); // 居中显示
    ui->lineEditCurrentPage->setAlignment(Qt::AlignCenter); // 居中显示
    ui->lineEditMaxRows->setReadOnly(true);
    ui->lineEditCurrentPage->setReadOnly(true);
    ui->pushButtonFront->setEnabled(true);
    ui->pushButtonNext->setEnabled(true);
    // 信号槽连接
    connect(ui->pushButtonFront, &QPushButton::clicked, this, &PageFlipWidget::previousPage);
    connect(ui->pushButtonNext, &QPushButton::clicked, this, &PageFlipWidget::nextPage);
    connect(ui->lineEditCurrentPage, &ComboLine::itemSelected, this, &PageFlipWidget::onCurrentPageSelected);
    connect(ui->lineEditMaxRows, &ComboLine::itemSelected, this, &PageFlipWidget::onMaxRowPerPageSelected);
    // 样式美化
    ui->lineEditCurrentPage->setProperty("style_frame", "bottomframe");
    ui->lineEditMaxRows->setProperty("style_frame", "bottomframe");
}

PageFlipWidget::~PageFlipWidget()
{
    delete ui;
}

int PageFlipWidget::totalRow() const
{
    return m_totalRow;
}

int PageFlipWidget::maxRowPerPage() const
{
    return m_maxRowPerPage;
}

int PageFlipWidget::pageCount() const
{
    if (m_maxRowPerPage <= 0) return 1;
    return qCeil(m_totalRow * 1.0 / m_maxRowPerPage);
}

int PageFlipWidget::isFirstPage() const
{
    return m_currentPage == 1;
}

int PageFlipWidget::isLastPage() const
{
    return m_currentPage == pageCount();
}

void PageFlipWidget::setTotalRow(int newTotalRow)
{
    m_totalRow = newTotalRow;
    ui->labelTotalCount->setText(QString("共%1条，每页").arg(m_totalRow));
    reset();
}

void PageFlipWidget::setMaxRowPerPage(int newMaxRowPerPage)
{
    if (newMaxRowPerPage <= 0) return;
    // 获取下拉列表中的值，判断传入值是否合法
    QStringList items = ui->lineEditMaxRows->items();
    for (const auto& item : std::as_const(items)) {
        if (item.toInt() == newMaxRowPerPage) {
            m_maxRowPerPage = newMaxRowPerPage;
            reset();
            break;
        }
    }
}

void PageFlipWidget::setRowPerPageItems(const QList<int> &items)
{
    ui->lineEditMaxRows->setItems(items);
    if (items.size() > 0) {
        ui->lineEditMaxRows->setText(QString::number(items[0]));
        setMaxRowPerPage(items[0]);
    }
}

void PageFlipWidget::nextPage()
{
    if (!isLastPage()) {
        switchPage(m_currentPage + 1);
    }
}

void PageFlipWidget::previousPage()
{
    if (!isFirstPage()) {
        switchPage(m_currentPage - 1);
    }
}

void PageFlipWidget::jumpToPage(int page)
{
    if (page != m_currentPage && page > 0 && page <= pageCount()) {
        switchPage(page);
    }
}

void PageFlipWidget::onCurrentPageSelected(const QString &text)
{
    if (m_currentPage != text.toInt()) { // 页码变化时才切换
        jumpToPage(text.toInt());
    }
}

void PageFlipWidget::onMaxRowPerPageSelected(const QString &text)
{
    setMaxRowPerPage(text.toInt());
}

void PageFlipWidget::setPageCount()
{
    int count = pageCount();
    ui->labelPageCount->setText("/" + QString::number(count));
    ui->lineEditCurrentPage->setItems(1, count); // 设置页码下拉列表
    ui->lineEditCurrentPage->setText("1"); // 设置当前页码
}

void PageFlipWidget::reset()
{
    setPageCount(); // 设置页码显示
    switchPage(1); // 切换到首页
}

void PageFlipWidget::switchPage(int newCurrentPage)
{
    m_currentPage = newCurrentPage;
    ui->lineEditCurrentPage->setText(QString::number(m_currentPage));
    ui->pushButtonFront->setEnabled(!isFirstPage());
    ui->pushButtonNext->setEnabled(!isLastPage());

    int start = (m_currentPage - 1) * m_maxRowPerPage; // 起始行号(这是用来在Objectsmodel中用来索引)
    int leftLength = m_totalRow - start; // 剩余行数(可能不足一页)
    int finalLength = qMin(leftLength, m_maxRowPerPage); // 最终行数: 剩余行数 和 每页最大行数 的最小值
    emit pageNumberChanged(start, finalLength); // 发出信号让桶列表/对象列表更新
}
