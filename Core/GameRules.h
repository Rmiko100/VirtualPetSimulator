#pragma once

// 课程设计中的可调规则。日历、属性、互动、成长分别集中在一处。
struct GameRules {
    static constexpr int minutesPerHour = 60;
    static constexpr int hoursPerDay = 24;
    static constexpr int startHour = 8;

    static constexpr int minStat = 0;
    static constexpr int maxStat = 100;
    static constexpr int initialSatiety = 50;
    static constexpr int initialHealth = 100;
    static constexpr int initialMood = 80;
    static constexpr int initialEnergy = 70;
    static constexpr int initialHygiene = 80;
    static constexpr int initialIntimacy = 0;
    static constexpr int initialLevel = 1;
    static constexpr int initialExperience = 0;

    static constexpr int feedSatietyGain = 20;
    static constexpr int restEnergyGain = 30;
    static constexpr int cleanHygieneGain = 25;
    static constexpr int strokeIntimacyGain = 5;
    static constexpr int strokeMoodGain = 5;
    static constexpr int catPlayMoodGain = 15;
    static constexpr int catPlayEnergyCost = 8;
    static constexpr int dogPlayMoodGain = 20;
    static constexpr int dogPlayEnergyCost = 15;
    static constexpr int rabbitPlayMoodGain = 12;
    static constexpr int rabbitPlayEnergyCost = 6;

    static constexpr int feedMinutes = 15;
    static constexpr int playMinutes = 30;
    static constexpr int restMinutes = 120;
    static constexpr int cleanMinutes = 15;
    static constexpr int strokeMinutes = 5;
    static constexpr int waitMinutes = 60;

    static constexpr int feedExperience = 10;
    static constexpr int playExperience = 20;
    static constexpr int restExperience = 5;
    static constexpr int cleanExperience = 10;
    static constexpr int strokeExperience = 5;
    static constexpr int waitExperience = 0;

    static constexpr int hourlySatietyLoss = 5;
    static constexpr int hourlyMoodLoss = 2;
    static constexpr int hourlyEnergyLoss = 3;
    static constexpr int hourlyHygieneLoss = 2;
    static constexpr int lowStatThreshold = 20;
    static constexpr int lowHealthThreshold = 30;
    static constexpr int hungerHealthLoss = 5;
    static constexpr int hygieneHealthLoss = 3;

    static constexpr int experiencePerLevel = 100;
    static constexpr int juvenileLevel = 5;
    static constexpr int adultLevel = 10;
};
