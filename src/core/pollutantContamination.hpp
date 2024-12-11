#pragma once

#include <QtWidgets>

#define RED 0xFF0000
#define AMBER 0xFFB000
#define GREEN 0x00FF00
#define GREY 0x808080

/**
 * The compliance levels structure
 *
 * The <low> and <high> regions segment the middle region, from the <low> and <high> regions.
 * The <veryLow> value determines the strongest green value on the colour gradient.
 * The <veryHigh> value determines the strongest red value on the colour gradient.
 * While the strongest amber colour value is marked at the midpoint between the low and high value.
 */
struct ComplianceLevels {
  qreal veryLow,  ///< Very low compliance level
      low,        ///< Low compliance level
      high,       ///< High compliance level
      veryHigh;   ///< Very high compliance level
};

struct PollutantContaminationPoint {
  QDateTime time;
  qreal concentration;
};