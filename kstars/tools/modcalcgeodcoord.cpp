/***************************************************************************
                          modcalcgeodcoord.cpp  -  description
                             -------------------
    begin                : Tue Jan 15 2002
    copyright            : (C) 2002 by Pablo de Vicente
    email                : vicente@oan.es
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "modcalcgeodcoord.h"

#include <QTextStream>

#include <kfiledialog.h>
#include <kglobal.h>
#include <kmessagebox.h>

#include "dms.h"
#include "geolocation.h"
#include "kstars.h"
#include "widgets/dmsbox.h"


modCalcGeodCoord::modCalcGeodCoord(QWidget *parentSplit)
        : QFrame(parentSplit) {

    QStringList ellipsoidList;
    ellipsoidList << "IAU76" << "GRS80" << "MERIT83" << "WGS84" << "IERS89";

    setupUi(this);

    spheRadio->setChecked(true);
    ellipsoidBox->insertItems(5,ellipsoidList);
    geoPlace = new GeoLocation();
    showLongLat();
    setEllipsoid(0);
    show();

    connect(Clear, SIGNAL(clicked()), this, SLOT(slotClearGeoCoords()));
    connect(Compute, SIGNAL(clicked()), this, SLOT(slotComputeGeoCoords()));

}

modCalcGeodCoord::~modCalcGeodCoord(){
    delete geoPlace;
}

void modCalcGeodCoord::showLongLat(void)
{

    KStars *ks = (KStars*) topLevelWidget()->parent();
    LongGeoBox->show( ks->geo()->lng() );
    LatGeoBox->show( ks->geo()->lat() );
    AltGeoBox->setText( QString("0.0") );
}

void modCalcGeodCoord::setEllipsoid(int index) {

    geoPlace->changeEllipsoid(index);

}

void modCalcGeodCoord::getCartGeoCoords (void)
{

    geoPlace->setXPos( KGlobal::locale()->readNumber(XGeoBox->text())*1000. );
    geoPlace->setYPos( KGlobal::locale()->readNumber(YGeoBox->text())*1000. );
    geoPlace->setZPos( KGlobal::locale()->readNumber(ZGeoBox->text())*1000. );
}

void modCalcGeodCoord::getSphGeoCoords (void)
{
    geoPlace->setLong( LongGeoBox->createDms() );
    geoPlace->setLat(  LatGeoBox->createDms() );
    geoPlace->setHeight( AltGeoBox->text().toDouble() );
}

void modCalcGeodCoord::slotClearGeoCoords (void)
{

    geoPlace->setLong( 0.0 );
    geoPlace->setLat(  0.0 );
    geoPlace->setHeight( 0.0 );
    LatGeoBox->clearFields();
    LongGeoBox->clearFields();
}

void modCalcGeodCoord::slotComputeGeoCoords (void)
{

    if(cartRadio->isChecked()) {
        getCartGeoCoords();
        showSpheGeoCoords();
    } else {
        getSphGeoCoords();
        showCartGeoCoords();
    }

}

void modCalcGeodCoord::showSpheGeoCoords(void)
{
    LongGeoBox->show( geoPlace->lng() );
    LatGeoBox->show( geoPlace->lat() );
    AltGeoBox->setText( KGlobal::locale()->formatNumber( geoPlace->height(), 3) );
}

void modCalcGeodCoord::showCartGeoCoords(void)
{

    XGeoBox->setText( KGlobal::locale()->formatNumber( geoPlace->xPos()/1000. ,6));
    YGeoBox->setText( KGlobal::locale()->formatNumber( geoPlace->yPos()/1000. ,6));
    ZGeoBox->setText( KGlobal::locale()->formatNumber( geoPlace->zPos()/1000. ,6));
}

void modCalcGeodCoord::geoCheck(void) {

    XGeoBoxBatch->setEnabled( false );
    XGeoCheckBatch->setChecked( false );
    YGeoBoxBatch->setEnabled( false );
    YGeoCheckBatch->setChecked( false );
    YGeoBoxBatch->setEnabled( false );
    YGeoCheckBatch->setChecked( false );
    xyzInputCoords = false;
}

void modCalcGeodCoord::xyzCheck(void) {

    LongGeoBoxBatch->setEnabled( false );
    LongGeoCheckBatch->setChecked( false );
    LatGeoBoxBatch->setEnabled( false );
    LatGeoCheckBatch->setChecked( false );
    AltGeoBoxBatch->setEnabled( false );
    AltGeoCheckBatch->setChecked( false );
    xyzInputCoords = true;

}

void modCalcGeodCoord::slotLongCheckedBatch(){

    if ( LongGeoCheckBatch->isChecked() ) {
        LongGeoBoxBatch->setEnabled( false );
        geoCheck();
    } else {
        LongGeoBoxBatch->setEnabled( true );
    }
}

void modCalcGeodCoord::slotLatCheckedBatch(){

    if ( LatGeoCheckBatch->isChecked() ) {
        LatGeoBoxBatch->setEnabled( false );
        geoCheck();
    } else {
        LatGeoBoxBatch->setEnabled( true );
    }
}

void modCalcGeodCoord::slotElevCheckedBatch(){

    if ( AltGeoCheckBatch->isChecked() ) {
        AltGeoBoxBatch->setEnabled( false );
        geoCheck();
    } else {
        AltGeoBoxBatch->setEnabled( true );
    }
}

void modCalcGeodCoord::slotXCheckedBatch(){

    if ( XGeoCheckBatch->isChecked() ) {
        XGeoBoxBatch->setEnabled( false );
        xyzCheck();
    } else {
        XGeoBoxBatch->setEnabled( true );
    }
}

void modCalcGeodCoord::slotYCheckedBatch(){

    if ( YGeoCheckBatch->isChecked() ) {
        YGeoBoxBatch->setEnabled( false );
        xyzCheck();
    } else {
        YGeoBoxBatch->setEnabled( true );
    }
}

void modCalcGeodCoord::slotZCheckedBatch(){

    if ( ZGeoCheckBatch->isChecked() ) {
        ZGeoBoxBatch->setEnabled( false );
        xyzCheck();
    } else {
        ZGeoBoxBatch->setEnabled( true );
    }
}
void modCalcGeodCoord::slotInputFile() {

    QString inputFileName;
    inputFileName = KFileDialog::getOpenFileName( );
    InputFileBoxBatch->setUrl( inputFileName );
}

void modCalcGeodCoord::slotOutputFile() {

    QString outputFileName;
    outputFileName = KFileDialog::getSaveFileName( );
    OutputFileBoxBatch->setUrl( outputFileName );
}

void modCalcGeodCoord::slotRunBatch(void) {

    QString inputFileName;

    inputFileName = InputFileBoxBatch->url().toLocalFile();

    // We open the input file and read its content

    if ( QFile::exists(inputFileName) ) {
        QFile f( inputFileName );
        if ( !f.open( QIODevice::ReadOnly) ) {
            QString message = i18n( "Could not open file %1.", f.fileName() );
            KMessageBox::sorry( 0, message, i18n( "Could Not Open File" ) );
            inputFileName.clear();
            return;
        }

        //		processLines(&f);
        QTextStream istream(&f);
        processLines(istream);
        //		readFile( istream );
        f.close();
    } else  {
        QString message = i18n( "Invalid file: %1", inputFileName );
        KMessageBox::sorry( 0, message, i18n( "Invalid file" ) );
        inputFileName.clear();
        InputFileBoxBatch->setUrl( inputFileName );
        return;
    }
}

void modCalcGeodCoord::processLines( QTextStream &istream ) {

    // we open the output file

    //	QTextStream istream(&fIn);
    QString outputFileName;
    outputFileName = OutputFileBoxBatch->url().toLocalFile();
    QFile fOut( outputFileName );
    fOut.open(QIODevice::WriteOnly);
    QTextStream ostream(&fOut);

    QString line;
    QString space = " ";
    int i = 0;
    GeoLocation geoPl;
    geoPl.setEllipsoid(0);

    double xB, yB, zB, hB;
    dms latB, longB;


    while ( ! istream.atEnd() ) {
        line = istream.readLine();
        line.trimmed();

        //Go through the line, looking for parameters

        QStringList fields = line.split( ' ' );

        i = 0;

        // Input coords are XYZ:

        if (xyzInputCoords) {

            // Read X and write in ostream if corresponds

            if(XGeoCheckBatch->isChecked() ) {
                xB = fields[i].toDouble();
                i++;
            } else
                xB = KGlobal::locale()->readNumber(XGeoBoxBatch->text()) ;

            if ( AllRadioBatch->isChecked() )
                ostream << xB << space;
            else
                if(XGeoCheckBatch->isChecked() )
                    ostream << xB << space;

            // Read Y and write in ostream if corresponds

            if(YGeoCheckBatch->isChecked() ) {
                yB = fields[i].toDouble();
                i++;
            } else
                yB = KGlobal::locale()->readNumber( YGeoBoxBatch->text()) ;

            if ( AllRadioBatch->isChecked() )
                ostream << yB << space;
            else
                if(YGeoCheckBatch->isChecked() )
                    ostream << yB << space;
            // Read Z and write in ostream if corresponds

            if(ZGeoCheckBatch->isChecked() ) {
                zB = fields[i].toDouble();
                i++;
            } else
                zB = KGlobal::locale()->readNumber( ZGeoBoxBatch->text());

            if ( AllRadioBatch->isChecked() )
                ostream << zB << space;
            else
                if(YGeoCheckBatch->isChecked() )
                    ostream << zB << space;

            geoPl.setXPos( xB*1000.0 );
            geoPl.setYPos( yB*1000.0 );
            geoPl.setZPos( zB*1000.0 );
            ostream << geoPl.lng()->toDMSString() << space <<
            geoPl.lat()->toDMSString() << space <<
            geoPl.height() << endl;

            // Input coords. are Long, Lat and Height

        } else {

            // Read Longitude and write in ostream if corresponds

            if(LongGeoCheckBatch->isChecked() ) {
                longB = dms::fromString( fields[i],true);
                i++;
            } else
                longB = LongGeoBoxBatch->createDms(true);

            if ( AllRadioBatch->isChecked() )
                ostream << longB.toDMSString() << space;
            else
                if(LongGeoCheckBatch->isChecked() )
                    ostream << longB.toDMSString() << space;

            // Read Latitude and write in ostream if corresponds

            if(LatGeoCheckBatch->isChecked() ) {
                latB = dms::fromString( fields[i], true);
                i++;
            } else
                latB = LatGeoBoxBatch->createDms(true);

            if ( AllRadioBatch->isChecked() )
                ostream << latB.toDMSString() << space;
            else
                if(LatGeoCheckBatch->isChecked() )
                    ostream << latB.toDMSString() << space;

            // Read Height and write in ostream if corresponds

            if(AltGeoCheckBatch->isChecked() ) {
                hB = fields[i].toDouble();
                i++;
            } else
                hB = AltGeoBoxBatch->text().toDouble() ;

            if ( AllRadioBatch->isChecked() )
                ostream << hB << space;
            else
                if(AltGeoCheckBatch->isChecked() )
                    ostream << hB << space;

            geoPl.setLong( longB );
            geoPl.setLat(  latB );
            geoPl.setHeight( hB );

            ostream << geoPl.xPos()/1000.0 << space <<
            geoPl.yPos()/1000.0 << space <<
            geoPl.zPos()/1000.0 << endl;

        }

    }


    fOut.close();
}

#include "modcalcgeodcoord.moc"
