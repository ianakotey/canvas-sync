#include "clickable_tree_view.h"

void ClickableTreeView::context_menu(const QPoint &pos)
{
  const QModelIndex index = indexAt(pos);
  if (!index.isValid() || !index.parent().isValid()) return;

  // create menu
  QMenu menu;
  if (index.column() == TreeIndex::REMOTE_DIR) {
    menu.addAction("Track Folder");
  }
  if (!TreeIndex(index).local_dir().isEmpty()) {
    menu.addAction("Clear");
  }
  auto e = menu.exec(mapToGlobal(pos));
  if (e == nullptr) return;

  // get selected item
  QString target = e->text();

  if (target == "Track Folder") {
    emit track_folder(index);
  }
  if (target == "Clear") {
    emit cleared(index);
  }
}

ClickableTreeView::ClickableTreeView(QWidget *parent) : QTreeView(parent)
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QTreeView::customContextMenuRequested, this,
          [&](const QPoint &pos) { this->context_menu(pos); });
}

TreeModel *ClickableTreeView::model() const
{
  return static_cast<TreeModel *>(QTreeView::model());
};

void ClickableTreeView::setModel(TreeModel *model)
{
  QTreeView::setModel(model);
  this->expand_tracked();
};

bool expand_tracked_inner(ClickableTreeView *tv, const QModelIndex &index)
{
  if (!index.isValid()) return false;
  TreeModel *model = tv->model();
  bool expand = false;
  int n = model->childrenCount();
  for (int i = 0; i < n; i++) {
    QModelIndex child = model->index(i, 0, index);
    expand |= expand_tracked_inner(tv, child);
  }
  if (!TreeIndex(index).local_dir().isEmpty() || expand) {
    tv->expand(index.parent());
    return true;
  }
  return expand;
}

void ClickableTreeView::expand_tracked()
{
  TreeModel *model = this->model();
  int n = model->childrenCount();
  for (int i = 0; i < n; i++) {
    QModelIndex child = model->index(i, 0);
    if (expand_tracked_inner(this, child)) {
      this->expand(child);
    }
  }
}

void ClickableTreeView::prettify()
{
  this->resizeColumnToContents(TreeIndex::REMOTE_DIR);
  if (columnWidth(TreeIndex::REMOTE_DIR) > this->width() * 0.6) {
    setColumnWidth(TreeIndex::REMOTE_DIR, this->width() * 0.6);
  }
};
