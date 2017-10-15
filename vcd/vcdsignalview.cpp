#include "vcdsignalview.h"

VcdSignalView::VcdSignalView(QWidget *parent) :
	QGraphicsView(parent),
	signalScene(new QGraphicsScene(this)),
	timeScale(1),
	recentZeroTime(0),
	moveDragLastX(0)
{
	setAcceptDrops(true);
	signalScene->setBackgroundBrush(Qt::black);
	setScene(signalScene);
}


void VcdSignalView::onZoomIn()
{
	//timeScale++;
	//redraw();
	scale(1.1,1.0);
}

void VcdSignalView::onZoomOut()
{
	//if(timeScale>1) timeScale--;
	//redraw();
	scale(0.9,1.0);
}

void VcdSignalView::onZoomFitWidth()
{
	//timeScale=1;
	//redraw();
}

void VcdSignalView::onMoveRight()
{
	//recentZeroTime++;
	//redraw();
}

void VcdSignalView::onMoveLeft()
{
	//if(recentZeroTime>1) recentZeroTime--;
	//redraw();
}

void VcdSignalView::resizeEvent(QResizeEvent *event)
{
	//redraw();
}

void VcdSignalView::mousePressEvent(QMouseEvent *e)
{
	/*if (e->button() == Qt::MiddleButton)
	{
		QPointF pt = mapToScene(e->pos());
		moveDragLastX = pt.x();
	}*/
}

void VcdSignalView::mouseMoveEvent(QMouseEvent *e)
{
	/*int x;
	if (e->buttons() & Qt::MidButton)
	{
		QPointF pt = mapToScene(e->pos());
		x = pt.x();

		int newZeroTime=recentZeroTime+(x-moveDragLastX);
		int rightLimit=(timeScale*this->width()*highest_time/(highest_time-lowest_time))-signalScene->sceneRect().width();

		if((newZeroTime>=0)&&(newZeroTime<=rightLimit)) {
			recentZeroTime=newZeroTime;
			moveDragLastX=x;
		}
		redraw();
	}*/
}

void VcdSignalView::wheelEvent(QWheelEvent *event)
{
	/*int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	int newZeroTime=recentZeroTime+numSteps*100;
	int rightLimit=(timeScale*this->width()*highest_time/(highest_time-lowest_time))-signalScene->sceneRect().width();

	if((newZeroTime>=0)&&(newZeroTime<=rightLimit)) {
		recentZeroTime=newZeroTime;
		event->accept();
	}
	redraw();*/
}

void VcdSignalView::mouseReleaseEvent(QMouseEvent * e)
{
	//QPointF pt = mapToScene(e->pos());
	//int x = pt.x();
	//redraw();
	//signalScene->addLine(x,0,x,this->height(),QPen(Qt::red));

}

void VcdSignalView::mouseDoubleClickEvent(QMouseEvent * e)
{
	//QPointF pt = mapToScene(e->pos());
	//int x = pt.x();
	//redraw();
	//signalScene->addLine(x,0,x,this->height(),QPen(Qt::red));

}

void VcdSignalView::contextMenuEvent(QContextMenuEvent *e)
{
	int y1, y2;
	QPointF pt = mapToScene(e->pos());
	QMenu menu(this);
	QAction *action;

	foreach(QString key, busSignalAreas.keys()) {
		if(!signalViewFilter.contains(key)) continue;

		y1 = busSignalAreas[key].area.y();
		y2 = y1+busSignalAreas[key].area.height();
		if((pt.y()>=y1)&&(pt.y()<=y2)) {
			action = menu.addAction("Remove signal bus");
			action->setData(key);
			connect(action,SIGNAL(triggered(bool)),this, SLOT(onRemoveSignal()));

			if(busSignalAreas[key].isUnfolded) {
				action = menu.addAction("Fold bus");
				action->setData(key);
				connect(action,SIGNAL(triggered(bool)),this, SLOT(onFoldSignalBus()));
			} else {
				action = menu.addAction("Unfold bus");
				action->setData(key);
				connect(action,SIGNAL(triggered(bool)),this, SLOT(onUnFoldSignalBus()));
			}
		}
	}

	foreach(QString key, signalAreas.keys()) {
		y1 = signalAreas[key].y();
		y2 = y1+signalAreas[key].height();
		if((pt.y()>=y1)&&(pt.y()<=y2)) {
			action = menu.addAction("Remove signal");
			action->setData(key);
			connect(action,SIGNAL(triggered(bool)),this, SLOT(onRemoveSignal()));
		}
	}

	menu.exec(e->globalPos());
}

void VcdSignalView::onFoldSignalBus()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString key = action->data().toString();

	if(busSignalAreas.contains(key)) {
		busSignalAreas[key].isUnfolded = false;
	}
}

void VcdSignalView::onUnFoldSignalBus()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString key = action->data().toString();

	if(busSignalAreas.contains(key)) {
		busSignalAreas[key].isUnfolded = true;
	}
}

void VcdSignalView::onRemoveSignal()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QString key = action->data().toString();

	for(int i=0;i<signalViewFilter.count();i++) {
		if(signalViewFilter.at(i)==key) {
			signalViewFilter.removeAt(i);
		}
	}
}

QString VcdSignalView::longSignalID(std::vector<std::string> arr)
{
	QString ret;
	foreach(std::string s, arr) {
		ret+='/';
		ret+=QString::fromStdString(s);
	}
	return ret;
}

void VcdSignalView::append(QString s)
{
	foreach(vcd::Var var, vcd_data.vars()) {
		if(longSignalID(var.hierarchical_name())==s) {
			if(var.width()>1) {
				VcdViewGraphicsItemBus *bus = new VcdViewGraphicsItemBus(var,vcd_data.time_bus_values());
				signalScene->addItem(bus);
			} else {
				VcdViewGraphicsItemSignal *signal = new VcdViewGraphicsItemSignal(var,vcd_data.time_values());
				signalScene->addItem(signal);
			}
		}
	}
	update();
}

void VcdSignalView::setVCD(vcd::VcdData d)
{
	QString hname;
	vcd_data = d;
	mapIdName.clear();
	foreach(vcd::Var var, vcd_data.vars()) {
		hname = "";
		foreach(std::string sname, var.hierarchical_name()) {
			hname+='/';
			hname+=QString::fromStdString(sname);
		}
		mapIdName[hname]=var.id();

		busSignalAreas[hname].bus_width = var.width();
	}

	highest_time = 0;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(value.time()>highest_time) highest_time = value.time();
	}

	lowest_time = highest_time;
	foreach(vcd::TimeValue value, vcd_data.time_values()) {
		if(value.time()<lowest_time) lowest_time = value.time();
	}
}

QString VcdSignalView::getHierarchyNameString(std::vector<std::string> l)
{
	QString ret = "";
	std::string s;
	foreach(s,l) {
		ret+='/';
		ret+=QString::fromStdString(s);
	}
	return ret;
}

bool VcdSignalView::drawSignalBus(QString signal_name)
{
	if(!mapIdName.contains(signal_name)) return false;

	int height;
	int divisor;
	QVector<vcd::LogicValue> reverse;

	QGraphicsSimpleTextItem *text;
	int width = 1;
	foreach(vcd::Var var, vcd_data.vars()) {
		if(getHierarchyNameString(var.hierarchical_name())==signal_name) {
			width = var.width();
		}
	}

	bool drawn = false;

	divisor = signalViewFilter.count();
	foreach(QString key, busSignalAreas.keys()) {
		if(signalViewFilter.contains(key))
			if(busSignalAreas[key].isUnfolded)
				divisor += busSignalAreas[key].bus_width;
	}

	height = this->height()-10;
	height /= divisor;

	int space = height;
	space /= 10;

	QPen sigPen(Qt::green);

	int i;
	int lastTime = 0;
	int time;
	int raise_time;
	QString lastBusValueStd;

	char busValueStd[width+1];

	foreach(vcd::TimeBusValue bus, vcd_data.time_bus_values()) {
		if(bus.var_id()==mapIdName[signal_name]) {
			time = timeScale*this->width()*(bus.time()-lowest_time)/(highest_time-lowest_time);
			raise_time = (time-lastTime)/RAISE_TIME;

			busValueStd[width]='\0';
			for(i=0;i<width;i++) {
				busValueStd[i]='0';
			}

			reverse.clear();
			foreach(vcd::LogicValue value, bus.values())
				reverse.prepend(value);

			i = width-1;
			foreach(vcd::LogicValue value, reverse) {
				if(value==vcd::LogicValue::ONE) {
					busValueStd[i]='1';
				} else if(value==vcd::LogicValue::ZERO) {
					busValueStd[i]='0';
				}
				i--;
			}

			sigPen.setColor(Qt::green);
			if(lastBusValueStd != busValueStd) {
				signalScene->addLine(time+raise_time, drawingIndex*height+space*2, time-raise_time, (drawingIndex+1)*height-space*2, sigPen);
				signalScene->addLine(time-raise_time, drawingIndex*height+space*2, time+raise_time, (drawingIndex+1)*height-space*2, sigPen);
				signalScene->addLine(lastTime+raise_time, drawingIndex*height+space*2, time-raise_time, drawingIndex*height+space*2, sigPen);
				signalScene->addLine(lastTime+raise_time, (drawingIndex+1)*height-space*2, time-raise_time, (drawingIndex+1)*height-space*2, sigPen);

				text = signalScene->addSimpleText(QString("%1").arg(QString(busValueStd).toInt(0,2)));
				text->setPos(time, (drawingIndex*height)+(height/2));
				text->setBrush(QColor(Qt::white));
				text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

				lastBusValueStd = busValueStd;
			} else {
				signalScene->addLine(lastTime, drawingIndex*height+space*2, time+raise_time, drawingIndex*height+space*2, sigPen);
				signalScene->addLine(lastTime, (drawingIndex+1)*height-space*2, time+raise_time, (drawingIndex+1)*height-space*2, sigPen);
			}

			drawn = true;
			lastTime = time;
		}
	}

	if(drawn) {
		// box around bus signal:
		signalScene->addRect(0, drawingIndex*height+space, lastTime, height-space*2, QPen(Qt::white));

		// add it to the list
		busSignalAreas[signal_name].area = QRect(0, drawingIndex*height+space, lastTime, height-space*2);

		// name of the signal bus:
		text = signalScene->addSimpleText(signal_name);
		text->setPos(recentZeroTime+space, drawingIndex*height+space);
		text->setBrush(QColor(Qt::white));
		text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

		drawingIndex++;

		if(busSignalAreas[signal_name].isUnfolded) {
			drawSubSignals(signal_name);
		}

		return true;
	}

	return false;
}

bool VcdSignalView::drawSubSignals(QString signal_name)
{
	if(!mapIdName.contains(signal_name)) return false;

	bool drawn = false;
	int divisor;
	int height;
	QVector<vcd::LogicValue> reverse;


	int width = 1;
	foreach(vcd::Var var, vcd_data.vars()) {
		if(getHierarchyNameString(var.hierarchical_name())==signal_name) {
			width = var.width();
		}
	}

	divisor = signalViewFilter.count();
	foreach(QString key, busSignalAreas.keys()) {
		if(signalViewFilter.contains(key))
			if(busSignalAreas[key].isUnfolded)
				divisor += busSignalAreas[key].bus_width;
	}

	height = this->height()-10;
	height /= divisor;

	int space = height;
	space /= 10;

	QPen sigPen(Qt::green);

	QVector<vcd::LogicValue> lastValue(width);
	for(int i=0; i<width; i++) {
		lastValue[i] = vcd::LogicValue::UNKOWN;
	}

	int lastTime = 0;
	int time;

	int bus_idx;
	int idx;
	foreach(vcd::TimeBusValue bus, vcd_data.time_bus_values()) {
		if(bus.var_id()==mapIdName[signal_name]) {
			reverse.clear();
			foreach(vcd::LogicValue value, bus.values())
				reverse.prepend(value);

			idx = drawingIndex;
			time = timeScale*this->width()*(bus.time()-lowest_time)/(highest_time-lowest_time);
			bus_idx = 0;
			foreach(vcd::LogicValue value, reverse) {
				if(value!=lastValue[bus_idx]) {
					signalScene->addLine(time, idx*height+space*2, time, (idx+1)*height-space*2 , sigPen);
				}

				if(lastValue[bus_idx]==vcd::LogicValue::ONE) {
					sigPen.setColor(Qt::green);
					signalScene->addLine(lastTime, idx*height+space*2, time, idx*height+space*2 , sigPen);
				} else if (lastValue[bus_idx]==vcd::LogicValue::ZERO) {
					sigPen.setColor(Qt::green);
					signalScene->addLine(lastTime, (idx+1)*height-space*2, time, (idx+1)*height-space*2, sigPen);
				} else if (lastValue[bus_idx]==vcd::LogicValue::UNKOWN) {
					if(value==vcd::LogicValue::ONE) {
						sigPen.setColor(Qt::green);
						signalScene->addLine(lastTime, (idx+1)*height-space*2, time, (idx+1)*height-space*2, sigPen);
					} else if (value==vcd::LogicValue::ZERO) {
						sigPen.setColor(Qt::green);
						signalScene->addLine(lastTime, idx*height+space*2, time, idx*height+space*2 , sigPen);
					}
				}
				lastValue[bus_idx] = value;

				idx++;
				bus_idx++;
			}
			for(;bus_idx<width;bus_idx++) {
				sigPen.setColor(Qt::green);
				if (lastValue[bus_idx]==vcd::LogicValue::UNKOWN) {
					signalScene->addLine(lastTime, (idx+1)*height-space*2, time, (idx+1)*height-space*2, sigPen);
				} else {
					if(lastValue[bus_idx]==vcd::LogicValue::ONE) {
						signalScene->addLine(lastTime, idx*height+space*2, time, idx*height+space*2 , sigPen);
					} else if (lastValue[bus_idx]==vcd::LogicValue::ZERO) {
						signalScene->addLine(lastTime, (idx+1)*height-space*2, time, (idx+1)*height-space*2, sigPen);

					}
				}
				idx++;
			}

			lastTime = time;
			drawn = true;
		}
	}

	if(drawn) {
		// box around bus signals:
		idx = drawingIndex;
		for(bus_idx=0; bus_idx < width; bus_idx++) {
			signalScene->addRect(0, idx*height+space, lastTime, height-space*2, QPen(Qt::white));
			idx++;
		}
		drawingIndex = idx;
	}
}
