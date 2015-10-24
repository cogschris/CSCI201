#include "main_window.h"
#include <map>
#include <vector>
#include <string>
#include "webpage.h"
#include "myset.h"
#include "pageparser.h"
#include "mdpageparser.h"
#include "searcheng.h"
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include "msort.h"

using namespace std;

struct OutgoingComp {
    bool operator()(WebPage* lhs, WebPage* rhs) 
    { 
      return lhs->outgoing() <= rhs->outgoing(); 
    }
  };

struct IncomingComp {
    bool operator()(WebPage* lhs, WebPage* rhs) 
    { 
      return (lhs->incoming()) <= (rhs->incoming()); 
    }
  };

struct Alphabet {
    bool operator()(WebPage* lhs, WebPage* rhs) 
    { 
    	return (lhs->filename()) <= (rhs->filename());
    }
  };

MainWindow::MainWindow(string name)
{
	
	PageParser* parser = new MDPageParser;
	
	searching.add_parse_from_index_file(name, parser);

	setWindowTitle("Chris' Google");

	overallLayout = new QHBoxLayout();

	display = new QVBoxLayout();
	overallLayout->addLayout(display);

	label = new QLabel("Results (filename / incoming links / outgoing links) :");
	display->addWidget(label);

	filelist = new QListWidget();
	connect(filelist, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showpage(QListWidgetItem*)));
	display->addWidget(filelist);

	

	//refresh_button = new QPushButton("Refresh/Reload");
	///display->addWidget(refresh_button);

	filename = new QPushButton("Sort by filename");
	connect(filename, SIGNAL(clicked()), this, SLOT(displayresults()));
	display->addWidget(filename);

	num_in = new QPushButton("Sort by number of incoming links");
	connect(num_in, SIGNAL(clicked()), this, SLOT(displayresultsincoming()));
	display->addWidget(num_in);

	num_out = new QPushButton("Sort by number of outgoing links");
	connect(num_out, SIGNAL(clicked()), this, SLOT(displayresultsoutgoing()));
	display->addWidget(num_out);

	formLayout = new QVBoxLayout();
	overallLayout->addLayout(formLayout);

	Input_text = new QLabel("Please enter search term(s):");
	formLayout->addWidget(Input_text);

	search_text = new QLineEdit();
	
	formLayout->addWidget(search_text);



	AND_button = new QRadioButton("AND", this);
	formLayout->addWidget(AND_button);

	OR_button = new QRadioButton("OR", this);
	formLayout->addWidget(OR_button);

	SINGLE_button = new QRadioButton("SINGLE", this);
	formLayout->addWidget(SINGLE_button);
	SINGLE_button->setChecked(true);

	search_button = new QPushButton("Search");
	connect(search_button, SIGNAL(clicked()), this, SLOT(functions()));
	connect(search_text, SIGNAL(returnPressed()), this, SLOT(functions()));
	formLayout->addWidget(search_button);

	exiting = new QPushButton("Exit Application");
	connect(exiting, SIGNAL(clicked()), this, SLOT(closeapp()));
	formLayout->addWidget(exiting);
	
	setLayout(overallLayout);


	otherlayout = new QVBoxLayout();
	otherwin = new QWidget;

	

	webpagestuff = new QTextEdit();
	webpagestuff->setReadOnly(true);
	otherlayout->addWidget(webpagestuff);

	title = new QLabel("Remember, data is organized as follows: filename / number of incoming links / number of outgoing links");
	otherlayout->addWidget(title);


	labels = new QHBoxLayout();
	otherlayout->addLayout(labels);


	newin = new QLabel("Incoming Links:");
	labels->addWidget(newin);

	newout = new QLabel("Outgoing Links:");
	labels->addWidget(newout);

	outin = new QHBoxLayout();
	otherlayout->addLayout(outin);

	in = new QListWidget();
	connect(in, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showpage(QListWidgetItem*)));
	outin->addWidget(in);

	out = new QListWidget();
	connect(out, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showpage(QListWidgetItem*)));
	outin->addWidget(out);

	

	filename2 = new QPushButton("Sort by filename");
	connect(filename2, SIGNAL(clicked()), this, SLOT(sortalph()));
	otherlayout->addWidget(filename2);

	num_in2 = new QPushButton("Sort by incoming links");
	connect(num_in2, SIGNAL(clicked()), this, SLOT(sortinc()));
	otherlayout->addWidget(num_in2);

	num_out2 = new QPushButton("Sort by outgoing links");
	connect(num_out2, SIGNAL(clicked()), this, SLOT(sortout()));
	otherlayout->addWidget(num_out2);

	exitnew = new QPushButton("Exit");
	connect(exitnew, SIGNAL(clicked()), this, SLOT(hidepage()));
	otherlayout->addWidget(exitnew);

	otherwin->setLayout(otherlayout);



}

MainWindow::~MainWindow() {

}

void MainWindow::displayresults() {
	
	filelist->clear();
	std::set<WebPage*>::iterator it;
	showlist.clear();
	Alphabet comp;
	for (it = results.begin(); it != results.end(); ++it) {
		
		showlist.push_back(*it);
		//cout << (*it)->filename() << endl;
	}

	int alphsize = showlist.size();
	//cout << alphsize << endl;
	
	mergeSort(showlist, comp);
	//cout << "done" << endl;
	for (int i = 0; i < alphsize; i++) {	
		//cout << "fuck" << endl;
		QString temp;

		//temp += "\"";
		//cout << i << endl;
		//cout << showlist[i]->filename() << endl;
		temp += QString::fromStdString((showlist[i])->filename());
		temp += "  In: ";
		//cout << "damn" << endl;
		temp += QString::number((showlist[i])->incoming());
		temp += "  Out: ";
		//cout << "okay" << endl;
		temp += QString::number((showlist[i])->outgoing());
		//temp += "\"";
		filelist->addItem(temp);
		//cout << "shit" << endl;
	}
}

void MainWindow::displayresultsincoming() {
	filelist->clear();
	std::set<WebPage*>::iterator it;
	showlist.clear();
	IncomingComp comp;
	for (it = results.begin(); it != results.end(); ++it) {
		
		showlist.push_back(*it);
	}

	int incsize = showlist.size();
	//cout << alphsize << endl;
	mergeSort(showlist, comp);
	
	for (int i = 0; i < incsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showlist[i])->incoming());
		temp += "  Out: ";
		temp += QString::number((showlist[i])->outgoing());
		//temp += "\"";
		filelist->addItem(temp);
	}
}

void MainWindow::displayresultsoutgoing() {
	filelist->clear();
	std::set<WebPage*>::iterator it;
	showlist.clear();
	OutgoingComp comp;
	for (it = results.begin(); it != results.end(); ++it) {
		
		showlist.push_back(*it);
	}

	int outsize = showlist.size();
	//cout << alphsize << endl;
	mergeSort(showlist, comp);
	
	for (int i = 0; i < outsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showlist[i])->incoming());
		temp += "  Out: ";
		temp += QString::number((showlist[i])->outgoing());
		//temp += "\"";
		filelist->addItem(temp);
	}
}

void MainWindow::closeapp() {
	otherwin->hide();
	close();
}

void MainWindow::functions() {
	

	if (AND_button->isChecked()) {
		//AND_button->setChecked(false);
		if (search_text->text().isEmpty()) {
			
			return;
		}
		string temp;
		temp = search_text->text().toStdString();
		istringstream stream (temp);
		search_text->setText("");
		string loading;
		stream >> loading;
		for (int i = 0; i < int(loading.size()); i++) {
            if (loading[i] < 92) {
              loading[i] = loading[i] + 32;
            }
          }
		results = searching.ONE_function(loading);

		while (stream >> loading) {
			for (int i = 0; i < int(loading.size()); i++) {
	            if (loading[i] < 92) {
	              loading[i] = loading[i] + 32;
	            }
          	}

          	results = searching.AND_function(loading, results);
		}

	}

	else if (OR_button->isChecked()) {
		//OR_button->setChecked(false);
		if (search_text->text().isEmpty()) {
			
			return;
		}
		string temp;
		temp = search_text->text().toStdString();
		istringstream stream (temp);
		search_text->setText("");
		string loading;
		stream >> loading;
		for (int i = 0; i < int(loading.size()); i++) {
            if (loading[i] < 92) {
              loading[i] = loading[i] + 32;
            }
          }
		results = searching.ONE_function(loading);

		while (stream >> loading) {
			for (int i = 0; i < int(loading.size()); i++) {
	            if (loading[i] < 92) {
	              loading[i] = loading[i] + 32;
	            }
          	}

          	results = searching.OR_function(loading, results);
		}

	}

	else if (SINGLE_button->isChecked()) {
		//SINGLE_button->setChecked(false);
		if (search_text->text().isEmpty()) {
			
			return;
		}
		string temp;
		temp = search_text->text().toStdString();
		istringstream stream (temp);
		search_text->setText("");
		string loading;
		stream >> loading;
		for (int i = 0; i < int(loading.size()); i++) {
            if (loading[i] < 92) {
              loading[i] = loading[i] + 32;
            }
          }
        if (stream >> loading) {
        //	error();
        	return;
        }
		results = searching.ONE_function(loading);
	}

	displayresults();
}

void MainWindow::showpage(QListWidgetItem* translate) {
	//WebPage* population;
	string test;
	QString str = translate->text();
	istringstream stream (str.toStdString());
	stream >> test;
	for (int i = 0; i < int(showlist.size()); i++) {
		if (showlist[i]->filename() == test) {
			secondin = showlist[i] -> incoming_links();
			secondout = showlist[i] -> outgoing_links();
			//cout << showlist[i] -> outgoing() << endl;
			populate(showlist[i]);
		}
	}


	otherwin->show();
}

void MainWindow::hidepage() {
	otherwin->hide();
}

void MainWindow::populate(WebPage* web) {
	otherwin->setWindowTitle(QString::fromStdString(web->filename()));
	//ostream propugation(NULL); 
	stringstream propugation;
	propugation << *web;
	string str="";
	str =  propugation.str();
	webpagestuff->setText(QString::fromStdString(str));
	sortalph();
}

void MainWindow::sortalph() {

	showinlist.clear(); showoutlist.clear();
	in->clear(); out->clear();

	std::set<WebPage*>::iterator it;
	
	Alphabet comp;
	for (it = secondin.begin(); it != secondin.end(); ++it) {
		
		showinlist.push_back(*it);
	}

	for (it = secondout.begin(); it != secondout.end(); ++it) {
		
		showoutlist.push_back(*it);
		//cout << (*it)->incoming() << " " << (*it)->outgoing() << endl;
	}



	//cout << alphsize << endl;
	mergeSort(showinlist, comp);
	mergeSort(showoutlist, comp);

	int outinsize = showinlist.size();
	int outoutsize = showoutlist.size();

	for (int i = 0; i < outinsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showinlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showinlist[i])->incoming());
		temp += "  Out: ";
		temp += QString::number((showinlist[i])->outgoing());
		//temp += "\"";
		in->addItem(temp);
	}

	for (int i = 0; i < outoutsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showoutlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showoutlist[i])->incoming());
		//std::cout << (showoutlist[i])->incoming() << std::endl;
		temp += "  Out: ";
		temp += QString::number((showoutlist[i])->outgoing());
		//cout << (showoutlist[i])->outgoing() << endl;
		//temp += "\"";
		out->addItem(temp);
	}
	
	
}

void MainWindow::sortinc() {

	showinlist.clear(); showoutlist.clear();
	in->clear(); out->clear();

	std::set<WebPage*>::iterator it;
	
	IncomingComp comp;
	for (it = secondin.begin(); it != secondin.end(); ++it) {
		
		showinlist.push_back(*it);
	}

	for (it = secondout.begin(); it != secondout.end(); ++it) {
		
		showoutlist.push_back(*it);
		//cout << (*it)->incoming() << " " << (*it)->outgoing() << endl;
	}



	//cout << alphsize << endl;
	mergeSort(showinlist, comp);
	mergeSort(showoutlist, comp);

	int outinsize = showinlist.size();
	int outoutsize = showoutlist.size();

	for (int i = 0; i < outinsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showinlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showinlist[i])->incoming());
		temp += "  Out: ";
		temp += QString::number((showinlist[i])->outgoing());
		//temp += "\"";
		in->addItem(temp);
	}

	for (int i = 0; i < outoutsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showoutlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showoutlist[i])->incoming());
		//std::cout << (showoutlist[i])->incoming() << std::endl;
		temp += "  Out: ";
		temp += QString::number((showoutlist[i])->outgoing());
		//cout << (showoutlist[i])->outgoing() << endl;
		//temp += "\"";
		out->addItem(temp);
	}
	
	
}

void MainWindow::sortout() {

	showinlist.clear(); showoutlist.clear();
	in->clear(); out->clear();

	std::set<WebPage*>::iterator it;
	
	OutgoingComp comp;
	for (it = secondin.begin(); it != secondin.end(); ++it) {
		
		showinlist.push_back(*it);
	}

	for (it = secondout.begin(); it != secondout.end(); ++it) {
		
		showoutlist.push_back(*it);
		//cout << (*it)->incoming() << " " << (*it)->outgoing() << endl;
	}



	//cout << alphsize << endl;
	mergeSort(showinlist, comp);
	mergeSort(showoutlist, comp);

	int outinsize = showinlist.size();
	int outoutsize = showoutlist.size();

	for (int i = 0; i < outinsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showinlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showinlist[i])->incoming());
		temp += "  Out: ";
		temp += QString::number((showinlist[i])->outgoing());
		//temp += "\"";
		in->addItem(temp);
	}

	for (int i = 0; i < outoutsize; i++) {	

		QString temp;

		//temp += "\"";
		temp += QString::fromStdString((showoutlist[i])->filename());
		temp += "  In: ";
		temp += QString::number((showoutlist[i])->incoming());
		//std::cout << (showoutlist[i])->incoming() << std::endl;
		temp += "  Out: ";
		temp += QString::number((showoutlist[i])->outgoing());
		//cout << (showoutlist[i])->outgoing() << endl;
		//temp += "\"";
		out->addItem(temp);
	}
	
	
}