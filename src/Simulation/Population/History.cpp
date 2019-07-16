#include "History.h"
#include "Logger.h"


HistoryPoint::HistoryPoint(std::vector<Genome*> genomes, int gen) :
        generation(gen), mean(), top(), middle(), bottom(), percentiles({{}}) {
    /// These calculations assume and ordered set where 0 is worst, .size() is best.
    for (const Genome* g: genomes) {
        mean.x += g->fitness;
    }
    mean.x /= genomes.size();


    top    = GenomeValue(genomes[genomes.size() - 1]->fitness, genomes[genomes.size() - 1]->toString());
    middle = GenomeValue(genomes[genomes.size() / 2]->fitness, genomes[genomes.size() / 2]->toString());
    bottom = GenomeValue(genomes[0]->fitness, genomes[0]->toString());

    const double percentileSize = genomes.size() / NUM_PERCENTILES;
    for (unsigned int i = 0; i < NUM_PERCENTILES; i++) { // Approxmiate calculation
        const int percentileIndex = i * percentileSize;

        percentiles[i] = GenomeValue(genomes[percentileIndex]->fitness,
                                                   genomes[percentileIndex]->toString());
    }
}

std::string HistoryPoint::toString() const {
    std::string s = utility::numToStr<int>(generation) + ", " +
        mean.toString()   + ", " +
        top.toString()    + ", " +
        middle.toString() + ", " +
        bottom.toString() + ", ";
    for (const GenomeValue& p: percentiles) {
        s += p.toString() + ", ";
    }
    return s;
}

void History::addPoint(std::vector<Genome*> genomes, int gen) {
    const unsigned int len = data.size();
    if (len >= MAX_NUM_DATA) {
        for (unsigned int i = 1; i < len; i+=2) {
            data.erase(data.begin() + i);
        }
    }
    data.push_back(HistoryPoint(genomes, gen));
}

void History::writeToFile() const{
    LOG("Writing history to file is not yet supported.", LogDegree::WARNING);
}

void History::writeToConsole() const {
    for (const HistoryPoint h: data) {
        std::cout << h.toString() << '\n';
    }
}

#include "Shapes.h"
#include "Text.h"
void History::graph() const {
    const double N = this->data.size();
    DrawRectangle<Appearance::BLACK>(0.1, 0.1, 0.9, 0.9);

    if (N < 2) {
        DrawString<Appearance::WHITE>("Not Enough Data Yet.", 0.5, 0.5, by_percentage());
        return;
    }
    const double minX = 0.1;
    const double maxX = 0.9;
    const double minY = 0.1;
    const double maxY = std::max_element(data.begin(), data.end(), [](const HistoryPoint& a, const HistoryPoint& b) {
        return a.bottom.x < b.bottom.x;
    })->bottom.x;

    DrawString<Appearance::WHITE>("max=" + utility::numToStr<double>(maxY), 0.1, 0.11, by_percentage(), false, true);
    double xP = 0;
    std::vector<double> ysP = {};
    auto getPoint = [maxY, minY](const double& v) {return 1 - (v / maxY * (1 - 2*minY) + minY);};
    for (int i = 0; i < N; i++) {
        double x = i/(N-1) * (maxX - minX) + minX;
        std::vector<double> ys = {
            getPoint(data[i].bottom.x),
            getPoint(data[i].middle.x),
            getPoint(data[i].top.x),
            getPoint(data[i].mean.x),
            getPoint(data[i].percentiles[0].x),
            getPoint(data[i].percentiles[1].x),
            getPoint(data[i].percentiles[2].x),
            getPoint(data[i].percentiles[3].x),
            getPoint(data[i].percentiles[4].x),
            getPoint(data[i].percentiles[5].x),
            getPoint(data[i].percentiles[6].x),
            getPoint(data[i].percentiles[7].x),
            getPoint(data[i].percentiles[8].x),
            getPoint(data[i].percentiles[9].x),
        };
        if (N > 1) {
            if (i == 0) {
                xP = x;
                ysP = ys;
            }
            for (unsigned int j = 0; j < ys.size(); j++) {
                glLineWidth(4);
                switch(j) {
                    case 3:
                        DrawLine<Appearance::RED>(xP, ysP[j], x, ys[j]); break;
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                        glLineWidth(1);
                        DrawLine<Appearance::GRAY>(xP, ysP[j], x, ys[j]); break;
                    default:
                        DrawLine<Appearance::WHITE>(xP, ysP[j], x, ys[j]); break;
                }
            }
        }

        xP = x;
        ysP = ys;
    }
}
