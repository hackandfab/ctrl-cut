TEMLATE = lib
TARGET = libctrl-cut.so
CONFIG += dll
CONFIG += boost cups gsapi
CONFIG += rsvg
CONFIG += cairo-ps
CONFIG += gio
CONFIG += boost_thread
CONFIG += libxml++
CONFIG += magick++
QMAKE_LFLAGS += -shared -Wl,-soname,libctrl-cut.so
QMAKE_CXXFLAGS += -fPIC

INCLUDEPATH += 

include(common.pri)

DEFINES += ETLOG DEBUG=4 USE_GHOSTSCRIPT_API

HEADERS +=  ./encoder/PclEncoder.hpp \
           ./encoder/HPGLEncoder.hpp \
           ./FileParser.hpp \
           ./Document.hpp \
           ./util/Eps.hpp \
           ./util/2D.hpp \
           ./util/Measurement.hpp \
           ./util/Logger.hpp \
           ./util/PJL.hpp \
           ./util/Util.hpp \
           ./cut/model/Explode.hpp \
           ./cut/model/Cut.hpp \
           ./cut/geom/Views.hpp \
           ./cut/geom/Geometry.hpp \
           ./cut/geom/SegmentTree.hpp \
           ./cut/geom/Path.hpp \
           ./cut/geom/Iterators.hpp \
           ./cut/geom/Route.hpp \
           ./cut/geom/Concepts.hpp \
           ./cut/graph/WeightedGraph.hpp \
           ./cut/graph/SegmentGraph.hpp \
           ./cut/graph/GeometryGraph.hpp \
           ./cut/graph/DistanceGraph.hpp \
           ./cut/graph/Traverse.hpp \
           ./cut/graph/Algorithms.hpp \
           ./engrave/image/PPMFile.hpp \
           ./engrave/image/Image.hpp \
           ./engrave/image/AbstractImage.hpp \
           ./engrave/image/MMapMatrix.hpp \
           ./engrave/Engrave.hpp \
           ./engrave/dither/DitherJarvis.hpp \
           ./engrave/dither/DitherSierra2.hpp \
           ./engrave/dither/DitherBurke.hpp \
           ./engrave/dither/DitherStucki.hpp \
           ./engrave/dither/DitherBayer.hpp \
           ./engrave/dither/DitherFloydSteinberg.hpp \
           ./engrave/dither/DitherThreshold.hpp \
           ./engrave/dither/Dither.hpp \
           ./engrave/dither/DitherSierra3.hpp \
           ./tri_logger/nullstream.hpp \
           ./tri_logger/tri_logger.hpp \
           ./config/CutSettings.hpp \
           ./config/DocumentSettings.hpp \
           ./config/PathSettings.hpp \
           ./config/LaserCutter.hpp \
           ./config/Settings.hpp \
           ./config/EngraveSettings.hpp \
           ./svg/Svg2Ps.hpp \
           ./svg/SvgDocument.hpp \
           ./svg/SvgFix.hpp

SOURCES += ./encoder/HPGLEncoder.cpp \
           ./encoder/PclEncoder.cpp \
           ./PostscriptParser.cpp \
           ./util/Measurement.cpp \
           ./util/Logger.cpp \
           ./util/Eps.cpp \
           ./engrave/dither/DitherBayer.cpp \
           ./engrave/dither/DitherFloydSteinberg.cpp \
           ./engrave/dither/DitherBurke.cpp \
           ./engrave/dither/DitherThreshold.cpp \
           ./engrave/dither/DitherSierra2.cpp \
           ./engrave/dither/DitherStucki.cpp \
           ./engrave/dither/DitherJarvis.cpp \
           ./engrave/dither/Dither.cpp \
           ./engrave/dither/DitherSierra3.cpp \
           ./Document.cpp \
           ./tri_logger/tri_logger.cpp \
           ./config/EngraveSettings.cpp \
	   ./config/BitmapSettings.cpp \
           ./config/PathSettings.cpp \
           ./config/CutSettings.cpp \
           ./config/DocumentSettings.cpp \
           ./svg/SvgFix.cpp \
           ./svg/SvgDocument.cpp \
           ./svg/Svg2Ps.cpp