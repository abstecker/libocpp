#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "everest/logging.hpp"
#include <ocpp/v16/ocpp_types.hpp>

namespace ocpp {
namespace v16 {

class CompositeScheduleTestFixture : public testing::Test {
protected:
    void SetUp() override {
    }

    /**
     * TxDefaultProfile, stack #1: time-of-day limitation to 2 kW, recurring every day from 17:00h to 20:00h.
     *
     * This profile is Example #1 taken from the OCPP 2.0.1 Spec Part 2, page 241.
     */
    ChargingProfile createChargingProfile_Example1() {
        auto chargingRateUnit = ChargingRateUnit::W;
        auto chargingSchedulePeriod = std::vector<ChargingSchedulePeriod>{ChargingSchedulePeriod{0, 2000, 1}};
        auto duration = 1080;
        auto startSchedule = ocpp::DateTime("2024-01-17T17:00:00");
        float minChargingRate = 0;
        auto chargingSchedule =
            ChargingSchedule{chargingRateUnit, chargingSchedulePeriod, duration, startSchedule, minChargingRate};

        auto chargingProfileId = 1;
        auto stackLevel = 1;
        auto chargingProfilePurpose = ChargingProfilePurposeType::TxDefaultProfile;
        auto chargingProfileKind = ChargingProfileKindType::Absolute;
        auto recurrencyKind = RecurrencyKindType::Daily;
        return ChargingProfile{chargingProfileId,
                               stackLevel,
                               chargingProfilePurpose,
                               chargingProfileKind,
                               chargingSchedule,
                               {}, // transactionId
                               recurrencyKind,
                               ocpp::DateTime("2024-01-01T00:00:00"),
                               ocpp::DateTime("2024-03-19T00:00:00")};
    }

    ChargingProfile createChargingProfile_Example2() {
        auto chargingRateUnit = ChargingRateUnit::W;
        auto chargingSchedulePeriod = std::vector<ChargingSchedulePeriod>{ChargingSchedulePeriod{0, 999999, 1}};
        auto duration = 0;
        auto startSchedule = ocpp::DateTime("2020-01-19T00:00:00");
        float minChargingRate = 0;
        auto chargingSchedule =
            ChargingSchedule{chargingRateUnit, chargingSchedulePeriod, duration, startSchedule, minChargingRate};

        auto chargingProfileId = 11;
        auto stackLevel = 2;
        auto chargingProfilePurpose = ChargingProfilePurposeType::TxDefaultProfile;
        auto chargingProfileKind = ChargingProfileKindType::Recurring;
        auto recurrencyKind = RecurrencyKindType::Weekly;
        return ChargingProfile{chargingProfileId,
                               stackLevel,
                               chargingProfilePurpose,
                               chargingProfileKind,
                               chargingSchedule,
                               {}, // transactionId
                               recurrencyKind,
                               ocpp::DateTime("2024-01-01T00:00:00"),
                               ocpp::DateTime("2024-03-19T00:00:00")};
    }

    json getProfilieJson() {
        return json::parse(R"(
        {
            "chargingProfileId": 1,
            "chargingProfileKind": "Absolute",
            "chargingProfilePurpose": "TxDefaultProfile",
            "chargingSchedule": {
                "chargingRateUnit": "W",
                "chargingSchedulePeriod": [
                    {
                        "limit": 2000.0,
                        "numberPhases": 1,
                        "startPeriod": 0
                    }
                ],
                "duration": 1080,
                "minChargingRate": 0.0,
                "startSchedule": "2024-01-17T17:00:00.000Z"
            },
            "recurrencyKind": "Daily",
            "stackLevel": 1
        }
        )");
        // return ex1;
    }
};

TEST_F(CompositeScheduleTestFixture, ocpp_types__ChargingProfileToFromJson_matches) {
  ChargingProfile from_cp = createChargingProfile_Example1();

  json into_json;
  to_json(into_json, from_cp);
  ChargingProfile to_cp;
  from_json(into_json, to_cp);

  ASSERT_EQ(from_cp.chargingProfileId, to_cp.chargingProfileId);

  EVLOG_info << into_json;
  ASSERT_TRUE(true);
}

TEST_F(CompositeScheduleTestFixture, DeserializeInlineJSON) {
    ChargingProfile cp;
    json raw_json = getProfilieJson();

    json into_json;
    to_json(into_json, createChargingProfile_Example2());

    from_json(getProfilieJson(), cp);

    ASSERT_EQ(1, cp.chargingProfileId);

    ASSERT_TRUE(true);
}

} // namespace v16
} // namespace ocpp