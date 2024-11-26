#include <QtWidgets>

class NavBar : public QWidget
{
private:
    QVBoxLayout list;

    QLabel
        _pollutantOverview,
        _persistentOrganicPollutants,
        _environmentalLitterIndicators,
        _flourinatedCompounds,
        _complianceDashboard,
        _geographicalHotspots;
public:
    NavBar()
        : _pollutantOverview("Pollutant Overview")
        , _persistentOrganicPollutants("Persistent Organic Pollutants")
        , _environmentalLitterIndicators("Environmental Litter Indicators")
        , _flourinatedCompounds("Flourinated Compounds")
        , _complianceDashboard("Compliance Dashboard")
        , _geographicalHotspots("Geographical Hotspots")
    {
        QPalette pal;
        pal.setColor(QPalette::Window, Qt::blue);

        list.addWidget(&_pollutantOverview);
        list.addWidget(&_persistentOrganicPollutants);
        list.addWidget(&_environmentalLitterIndicators);
        list.addWidget(&_flourinatedCompounds);
        list.addWidget(&_complianceDashboard);
        list.addWidget(&_geographicalHotspots);

        setAutoFillBackground(true);
        setPalette(pal);

        setLayout(&list);
    }
};