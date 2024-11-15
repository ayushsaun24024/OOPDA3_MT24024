void Line::addTrain(const Time& time, bool isStoppingTrain) {
    if (!canAddTrain(time, isStoppingTrain)) {
        throw TimeConflictException("Time slot conflicts with existing schedule");
    }
    schedules.emplace_back(time, isStoppingTrain);
}

void Platform::addLine(int lineNumber) {
    if (lineNumber <= 0) {
        throw RailwayException("Line number must be positive");
    }
    if (findLine(lineNumber)) {
        throw RailwayException("Line already exists on this platform");
    }
    lines.push_back(std::make_unique<Line>(lineNumber));
}

void Platform::addLines(const std::vector<int>& lineNumbers) {
    if (lineNumbers.empty()) {
        throw RailwayException("No line numbers provided");
    }
    for (int lineNum : lineNumbers) {
        addLine(lineNum);
    }
}

Line* Platform::findLine(int lineNumber) {
    auto it = std::find_if(lines.begin(), lines.end(),
        [lineNumber](const auto& line) { return line->getLineNumber() == lineNumber; });
    return it != lines.end() ? it->get() : nullptr;
}
