#ifndef SCHEMATICSDATA_H
#define SCHEMATICSDATA_H

#include <istream>
#include <fstream>

#include <QString>
#include <QVector>
#include <QRect>
#include <QTextStream>
#include <QString>
#include <QTextStream>
#include <QGraphicsTextItem>

#include "schematicswire.h"
#include "schematicspart.h"

namespace schematics {
	class SchematicsScanner;
	class SchematicsData
	{
	public:
		SchematicsData(QString file);
		class SchematicsScanner *getLexer();

		// paper format:
		void setFormat(std::string format, int w, int h);
		qreal getPaperHeigth();
		qreal getPaperWidth();

		// wires:
		void addWire(std::string name, int x1, int y1, int x2, int y2);
		QVector<SchematicsWire> getWires();

		// components:
		void setRecentPartName(std::string name);
		void setRecentPartType(std::string name);
		void setRecentPartPosition(int x, int y);
		void storeRecentComponent();
		QVector<SchematicsPart> getParts();

	private:
		SchematicsPart recentComponent;

		class SchematicsScanner* lexer;
		class SchematicsParser* parser;

		bool trace_parsing;
		bool trace_scanning;
		QString streamname;

		QVector<SchematicsWire> wires;
		QVector<SchematicsPart> parts;

		int paperHeigth;
		int paperWidth;
		QString paperName;

	};
}

#endif // SCHEMATICSDATA_H
