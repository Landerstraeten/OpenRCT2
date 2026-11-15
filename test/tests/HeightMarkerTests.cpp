/*****************************************************************************
 * Copyright (c) 2014-2026 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include <gtest/gtest.h>
#include <openrct2/SpriteIds.h>
#include <openrct2/config/Config.h>
#include <openrct2/interface/Viewport.h>

using namespace OpenRCT2;

class HeightMarkerTests : public testing::Test
{
protected:
    void SetUp() override
    {
        auto& general = Config::Get().general;
        _showHeightAsUnits = general.showHeightAsUnits;
        _measurementFormat = general.measurementFormat;
    }

    void TearDown() override
    {
        auto& general = Config::Get().general;
        general.showHeightAsUnits = _showHeightAsUnits;
        general.measurementFormat = _measurementFormat;
    }

    static void UseLabels(bool showHeightAsUnits, MeasurementFormat measurementFormat)
    {
        auto& general = Config::Get().general;
        general.showHeightAsUnits = showHeightAsUnits;
        general.measurementFormat = measurementFormat;
    }

private:
    bool _showHeightAsUnits{};
    MeasurementFormat _measurementFormat{};
};

TEST_F(HeightMarkerTests, metric_labels_up_to_120_metres_come_from_g1)
{
    UseLabels(false, MeasurementFormat::metric);
    ASSERT_EQ(GetHeightMarkerImageIndex(0), SPR_HEIGHT_MARKER_BASE + 2 * 256);
    ASSERT_EQ(GetHeightMarkerImageIndex(80), SPR_HEIGHT_MARKER_BASE + 2 * 256 + 80);
}

TEST_F(HeightMarkerTests, metric_labels_from_121_metres_come_from_g2)
{
    UseLabels(false, MeasurementFormat::metric);
    ASSERT_EQ(GetHeightMarkerImageIndex(81), SPR_G2_HEIGHT_MARKER_METRIC_START);
    ASSERT_EQ(GetHeightMarkerImageIndex(82), SPR_G2_HEIGHT_MARKER_METRIC_START + 1);
    ASSERT_EQ(GetHeightMarkerImageIndex(127), SPR_G2_HEIGHT_MARKER_METRIC_END);
}

TEST_F(HeightMarkerTests, imperial_labels_come_from_g1)
{
    UseLabels(false, MeasurementFormat::imperial);
    ASSERT_EQ(GetHeightMarkerImageIndex(0), SPR_HEIGHT_MARKER_BASE + 1 * 256);
    ASSERT_EQ(GetHeightMarkerImageIndex(127), SPR_HEIGHT_MARKER_BASE + 1 * 256 + 127);
}

TEST_F(HeightMarkerTests, unit_labels_come_from_g1_in_any_measurement_format)
{
    UseLabels(true, MeasurementFormat::metric);
    ASSERT_EQ(GetHeightMarkerImageIndex(0), SPR_HEIGHT_MARKER_BASE);
    ASSERT_EQ(GetHeightMarkerImageIndex(127), SPR_HEIGHT_MARKER_BASE + 127);

    UseLabels(true, MeasurementFormat::imperial);
    ASSERT_EQ(GetHeightMarkerImageIndex(127), SPR_HEIGHT_MARKER_BASE + 127);
}
