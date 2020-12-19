/******************************************************************************
 * FIRESTARTER - A Processor Stress Test Utility
 * Copyright (C) 2020 TU Dresden, Center for Information Services and High
 * Performance Computing
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/\>.
 *
 * Contact: daniel.hackenberg@tu-dresden.de
 *****************************************************************************/

#pragma once

#include <firestarter/Optimizer/Problem.hpp>

#include <cassert>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <tuple>
#include <utility>

namespace firestarter::optimizer::problem {

class CLIArgumentProblem final : public firestarter::optimizer::Problem {

public:
  CLIArgumentProblem(
      std::function<void(std::vector<std::pair<std::string, unsigned>> const &)>
          &&changePayloadFunction,
      std::shared_ptr<firestarter::measurement::MeasurementWorker> const
          &measurementWorker,
      std::vector<std::string> const &metrics, std::chrono::seconds timeout,
      std::chrono::milliseconds startDelta, std::chrono::milliseconds stopDelta,
      std::vector<std::string> const &instructionGroups)
      : _changePayloadFunction(changePayloadFunction),
        _measurementWorker(measurementWorker), _metrics(metrics),
        _timeout(timeout), _startDelta(startDelta), _stopDelta(stopDelta),
        _instructionGroups(instructionGroups) {
    assert(_metrics.size() != 0);
  }

  ~CLIArgumentProblem() {}

  // return all available metrics for the individual
  std::map<std::string, firestarter::measurement::Summary>
  metrics(std::vector<unsigned> const &individual) override {
    // increment evaluation idx
    _fevals++;

    // change the payload
    assert(_instructionGroups.size() == individual.size());
    std::vector<std::pair<std::string, unsigned>> payload = {};
    auto it1 = _instructionGroups.begin();
    auto it2 = individual.begin();
    for (; it1 != _instructionGroups.end(); ++it1, ++it2) {
      payload.push_back(std::make_pair(*it1, *it2));
    }
    _changePayloadFunction(payload);

    // start the measurement
    _measurementWorker->startMeasurement();

    // wait for the measurement to finish
    std::this_thread::sleep_for(_timeout);

    // return the results
    return _measurementWorker->getValues(_startDelta, _stopDelta);
  }

  std::vector<double> fitness(
      std::map<std::string, firestarter::measurement::Summary> const &summaries)
      override {
    std::vector<double> values = {};

    for (auto const metricName : _metrics) {
      auto findName = [metricName](auto const &summary) {
        return metricName.compare(summary.first) == 0;
      };

      auto it = std::find_if(summaries.begin(), summaries.end(), findName);

      if (it == summaries.end()) {
        continue;
      }

      values.push_back(it->second.average);
    }

    return values;
  }

  // get the bounds of the problem
  std::vector<std::tuple<unsigned, unsigned>> getBounds() const override {
    auto tup = std::make_tuple<unsigned, unsigned>(0, 100);
    return std::vector<decltype(tup)>(_instructionGroups.size(), tup);
  }

  // get the number of objectives.
  std::size_t getNobjs() const override { return _metrics.size(); }

private:
  std::function<void(std::vector<std::pair<std::string, unsigned>> const &)>
      _changePayloadFunction;
  std::shared_ptr<firestarter::measurement::MeasurementWorker>
      _measurementWorker;
  std::vector<std::string> _metrics;
  std::chrono::seconds _timeout;
  std::chrono::milliseconds _startDelta;
  std::chrono::milliseconds _stopDelta;
  std::vector<std::string> _instructionGroups;
};

} // namespace firestarter::optimizer::problem
