INC = -I ./../
HEADERS_ANNEALINGSCHEDULE = \
	./../sa/AnnealingSchedule.h

HEADERS_ANNEALINGSCHEDULERATIODECREASE = \
	./../sa/AnnealingSchedule.h\
	./../sa/AnnealingScheduleRatioDecrease.h

HEADERS_CHANGECORNER = \
	./../cornersequence/CornerSequence.h\
	./../sa/ChangeCorner.h\
	./../sa/FloorplanState.h\
	./../sa/Operation.h\
	./../sa/State.h\
	./../utils/Utils.h

HEADERS_FLOORPLANSTATE = \
	./../cornersequence/CornerSequence.h\
	./../cornerstitching/Corner.h\
	./../floorplan/Macro.h\
	./../quadtree/Point.h\
	./../sa/FloorplanState.h\
	./../sa/State.h

HEADERS_LARGESTEMPTYSPACESHAPE = \
	./../cornersequence/CornerSequence.h\
	./../cornerstitching/CornerVerticalTilePlane.h\
	./../cornerstitching/TilePlane.h\
	./../cornerstitching/VerticalTilePlane.h\
	./../sa/CostFunction.h\
	./../sa/FloorplanState.h\
	./../sa/LargestEmptySpaceShape.h\
	./../sa/State.h

HEADERS_MACROSOCCUPIEDREGIONAREA = \
	./../cornersequence/CornerSequence.h\
	./../sa/CostFunction.h\
	./../sa/FloorplanState.h\
	./../sa/MacrosOccupiedRegionArea.h\
	./../sa/State.h

HEADERS_REFRESHFLOORPLANWINDOW = \
	./../cornersequence/CornerSequence.h\
	./../sa/FloorplanState.h\
	./../sa/RefreshFloorplanWindow.h\
	./../sa/SimulatedAnnealing.h\
	./../sa/State.h\
	./../sa/TemperatureListener.h\
	./../view/FloorplanWindow.h\
	./../view/Window.h

HEADERS_SIMULATEDANNEALING = \
	./../sa/AnnealingSchedule.h\
	./../sa/CostFunction.h\
	./../sa/Operation.h\
	./../sa/SimulatedAnnealing.h\
	./../sa/State.h\
	./../sa/TemperatureListener.h\
	./../utils/Utils.h

HEADERS_SIMULATEDANNEALING_TEST = \
	./../cornersequence/CornerSequence.h\
	./../floorplan/Floorplan.h\
	./../floorplan/Macro.h\
	./../floorplan/SortedMacros.h\
	./../sa/AnnealingSchedule.h\
	./../sa/AnnealingScheduleRatioDecrease.h\
	./../sa/ChangeCorner.h\
	./../sa/CostFunction.h\
	./../sa/FloorplanState.h\
	./../sa/LargestEmptySpaceShape.h\
	./../sa/MacrosOccupiedRegionArea.h\
	./../sa/Operation.h\
	./../sa/RefreshFloorplanWindow.h\
	./../sa/SimulatedAnnealing.h\
	./../sa/SimulatedAnnealing_test.h\
	./../sa/State.h\
	./../sa/SwapMacros.h\
	./../sa/TemperatureListener.h\
	./../utils/Utils.h\
	./../view/FloorplanWindow.h\
	./../view/Window.h

HEADERS_STATE = \
	./../sa/State.h

HEADERS_SWAPMACROS = \
	./../cornersequence/CornerSequence.h\
	./../sa/FloorplanState.h\
	./../sa/Operation.h\
	./../sa/State.h\
	./../sa/SwapMacros.h\
	./../utils/Utils.h

HEADERS_TEMPERATURELISTENER = \
	./../sa/TemperatureListener.h

all:
	make AnnealingSchedule.o
	make AnnealingScheduleRatioDecrease.o
	make ChangeCorner.o
	make FloorplanState.o
	make LargestEmptySpaceShape.o
	make MacrosOccupiedRegionArea.o
	make RefreshFloorplanWindow.o
	make SimulatedAnnealing.o
	make SimulatedAnnealing_test.o
	make State.o
	make SwapMacros.o
	make TemperatureListener.o
AnnealingSchedule.o: AnnealingSchedule.cpp $(HEADERS_ANNEALINGSCHEDULE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c AnnealingSchedule.cpp $(INC)
AnnealingScheduleRatioDecrease.o: AnnealingScheduleRatioDecrease.cpp $(HEADERS_ANNEALINGSCHEDULERATIODECREASE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c AnnealingScheduleRatioDecrease.cpp $(INC)
ChangeCorner.o: ChangeCorner.cpp $(HEADERS_CHANGECORNER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c ChangeCorner.cpp $(INC)
FloorplanState.o: FloorplanState.cpp $(HEADERS_FLOORPLANSTATE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c FloorplanState.cpp $(INC)
LargestEmptySpaceShape.o: LargestEmptySpaceShape.cpp $(HEADERS_LARGESTEMPTYSPACESHAPE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c LargestEmptySpaceShape.cpp $(INC)
MacrosOccupiedRegionArea.o: MacrosOccupiedRegionArea.cpp $(HEADERS_MACROSOCCUPIEDREGIONAREA)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c MacrosOccupiedRegionArea.cpp $(INC)
RefreshFloorplanWindow.o: RefreshFloorplanWindow.cpp $(HEADERS_REFRESHFLOORPLANWINDOW)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c RefreshFloorplanWindow.cpp $(INC)
SimulatedAnnealing.o: SimulatedAnnealing.cpp $(HEADERS_SIMULATEDANNEALING)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c SimulatedAnnealing.cpp $(INC)
SimulatedAnnealing_test.o: SimulatedAnnealing_test.cpp $(HEADERS_SIMULATEDANNEALING_TEST)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c SimulatedAnnealing_test.cpp $(INC)
State.o: State.cpp $(HEADERS_STATE)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c State.cpp $(INC)
SwapMacros.o: SwapMacros.cpp $(HEADERS_SWAPMACROS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c SwapMacros.cpp $(INC)
TemperatureListener.o: TemperatureListener.cpp $(HEADERS_TEMPERATURELISTENER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c TemperatureListener.cpp $(INC)
clean: 
	rm *.o