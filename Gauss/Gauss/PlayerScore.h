#pragma once
class PlayerScore
{
private:
	unsigned long int score;
	unsigned long int enemiesKilled;
	int highestLevelAchieved;
	float totalSecondsAlive;
public:
	PlayerScore() : score(0), enemiesKilled(0), highestLevelAchieved(1), totalSecondsAlive(1) { }
	virtual ~PlayerScore(){}

	inline const unsigned long int getScore() const { return this->score; }
	inline const unsigned long int getEnemiesKilled() const { return this->enemiesKilled; }
	inline const int getHighestLevelAchieved() const { return this->highestLevelAchieved; }
	inline const float getSecondsSurvived() const { return this->totalSecondsAlive; }
	inline void setScore(unsigned long int score){ this->score = score; }
	inline void setEnemiesKilled(int enemiesKilled) { this->enemiesKilled = enemiesKilled; }
	inline void setHighestLevelAchieved(int highestLevelAchieved) { this->highestLevelAchieved = highestLevelAchieved; }
	inline void setSecondsSurvive(float totalSecondsAlive) { this->totalSecondsAlive = totalSecondsAlive; }
};

