/***************************************************************************
 *   Copyright (C) 1998-2009 by authors (see AUTHORS.txt )                 *
 *                                                                         *
 *   This file is part of LuxRender.                                       *
 *                                                                         *
 *   Lux Renderer is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   Lux Renderer is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 *   This project is based on PBRT ; see http://www.pbrt.org               *
 *   Lux Renderer website : http://www.luxrender.net                       *
 ***************************************************************************/

#ifndef LUX_SPPMSTATISTICS_H
#define LUX_SPPMSTATISTICS_H

#include "rendererstatistics.h"
#include "renderers/sppmrenderer.h"

#include <algorithm>

namespace lux
{

class SPPMRStatistics : public RendererStatistics {
public:
	SPPMRStatistics(SPPMRenderer* renderer);
	~SPPMRStatistics();

	class FormattedLong : public RendererStatistics::FormattedLong {
	public:
		FormattedLong(SPPMRStatistics* rs);

	private:
		SPPMRStatistics* rs;

		virtual std::string getRecommendedStringTemplate();

		std::string getPassCount();
		std::string getHaltPass();
		std::string getRemainingPasses();
		std::string getPercentHaltPassesComplete();
		std::string getPhotonCount();

		std::string getAveragePassesPerSecond();
		std::string getAveragePassesPerSecondWindow();
		std::string getAveragePhotonsPerSecond();
		std::string getAveragePhotonsPerSecondWindow();

		friend class SPPMRStatistics;
		friend class SPPMRStatistics::FormattedShort;
	};

	class FormattedShort : public RendererStatistics::FormattedShort {
	public:
		FormattedShort(SPPMRStatistics* rs);

	private:
		SPPMRStatistics* rs;

		virtual std::string getRecommendedStringTemplate();

		std::string getPassCount();
		std::string getHaltPass();
		std::string getRemainingPasses();
		std::string getPercentHaltPassesComplete();
		std::string getPhotonCount();
	};

private:
	SPPMRenderer* renderer;

	double windowPps;
	double windowYps;
	double windowPassCount;
	double windowPhotonCount;
	double windowPassStartTime;
	double windowPhotonStartTime;

	virtual void resetDerived();
	virtual void updateStatisticsWindowDerived();

	virtual double getPercentComplete() { return std::max(getPercentHaltTimeComplete(), getPercentHaltPassesComplete()); }
	virtual u_int getThreadCount() { return renderer->renderThreads.size(); }

	double getPassCount() { return renderer->hitPoints ? renderer->hitPoints->GetPassCount() : 0.0; }
	double getAveragePassesPerSecond();
	double getAveragePassesPerSecondWindow() { return windowPps; }

	double getHaltPass();
	double getRemainingPasses() { return std::max(0.0, getHaltPass() - getPassCount()); }
	double getPercentHaltPassesComplete();

	double getEfficiency() { return renderer->photonHitEfficiency; }

	double getPhotonCount() { return double(renderer->photonTracedTotal + renderer->photonTracedPass); }
	double getAveragePhotonsPerSecond();
	double getAveragePhotonsPerSecondWindow() { return windowYps; }
};

}//namespace lux

#endif // LUX_SPPMSTATISTICS_H