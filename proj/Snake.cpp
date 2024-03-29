
#include "Snake.h"


Snake::Snake(int fieldSizeX, int fieldSizeY, int moveDelay)
{
  randomSeed(analogRead(0));
  _fieldSizeX = fieldSizeX;
  _fieldSizeY = fieldSizeY;

  food = new pixel[1];

  snakeLimbs = new pixel[_fieldSizeX * _fieldSizeY];
  _snakeLength = 1;
  snakeLimbs[0].posX = _fieldSizeX / 2;
  snakeLimbs[0].posY = _fieldSizeY / 2;

  _bodyColor.set(0, 255, 0);
  _headColor.set(0, 240, 60);
  _foodColor.set(255, 0, 0);
  setColor(_bodyColor, 1);
  setColor(_headColor, 2);
  setColor(_foodColor, 3);

  relocateFood();


  _direction = RIGHT;
  _wasReset = false;
  _moveDelay = moveDelay;
  _snakeTimer = _moveDelay;
}


bool Snake::checkMove(pixel newPos)
{
  if (isPointOnSnake(newPos))
    return false;
  if (newPos.posX < 0 || newPos.posX >= _fieldSizeX)
    return false;
  if (newPos.posY < 0 || newPos.posY >= _fieldSizeY)
    return false;

  return true;
}

void Snake::resetGame()
{
  _snakeLength = 1;
  snakeLimbs[0].posX = _fieldSizeX / 2;
  snakeLimbs[0].posY = _fieldSizeY / 2;
  _wasReset = true;
}


void Snake::makeMove(byte direction)
{

  for (int i = _snakeLength - 1; i >= 0; i--)
  {
    if (i == 0)
    {

      pixel newPos = snakeLimbs[0];
      switch (direction)
      {
        case LEFT:
          newPos.posX--;
          break;
        case RIGHT:
          newPos.posX++;
          break;
        case UP:
          newPos.posY++;
          break;
        case DOWN:
          newPos.posY--;
          break;
      }

      _last_direction = direction;

      if (checkMove(newPos))
      {
        snakeLimbs[0] = newPos;
      }
      else
        resetGame();
    }
    else
    {
      snakeLimbs[i].posX = snakeLimbs[i - 1].posX;
      snakeLimbs[i].posY = snakeLimbs[i - 1].posY;
    }
  }

}



void Snake::tick()
{
  _snakeTimer--;
  if (_snakeTimer == 0)
  {
    makeMove(_direction);
    checkFood();
    _snakeTimer = _moveDelay;
  }
}

void Snake::goLeft()
{
  if (_last_direction != RIGHT) {
    _direction = LEFT;
  }

}

void Snake::goRight()
{
  if (_last_direction != LEFT) {
    _direction = RIGHT;
  }
}

void Snake::goUp()
{
  if (_last_direction != DOWN) {
    _direction = UP;
  }

}

void Snake::goDown()
{
  if (_last_direction != UP) {
    _direction = DOWN;
  }

}

int Snake::getSnakeLenght()
{
  return _snakeLength;
}

Snake::pixel* Snake::getSnakeLimbs()
{
  return snakeLimbs;
}

void Snake::increaseSize()
{
  if (_snakeLength < _fieldSizeX * _fieldSizeY)
  {
    _snakeLength++;
    snakeLimbs[_snakeLength - 1].posX = snakeLimbs[_snakeLength - 2].posX;
    snakeLimbs[_snakeLength - 1].posY = snakeLimbs[_snakeLength - 2].posY;
    snakeLimbs[_snakeLength - 1].pixelColor = _bodyColor;
  }
}

bool Snake::wasGameReset()
{
  if (_wasReset)
  {
    _wasReset = false;
    return true;
  }
  else
  {
    return false;
  }
}

void Snake::checkFood()
{
  if (snakeLimbs[0].posX == food[0].posX && snakeLimbs[0].posY == food[0].posY)
  {
    increaseSize();
    relocateFood();
  }

}

Snake::pixel* Snake::getFoodPositions()
{
  return food;
}

void Snake::relocateFood()
{
  do
  {
    food[0].posX = random(0, 11);
    food[0].posY = random(0, 11);
  } while (isPointOnSnake(food[0]));
}

bool Snake::isPointOnSnake(pixel pointPos)
{
  for (int i = 0; i < _snakeLength; i++)
  {
    if (pointPos.posX == snakeLimbs[i].posX && pointPos.posY == snakeLimbs[i].posY)
    {
      return true;
    }
  }
  return false;
}

void Snake::setColor(colorRGB color, byte destination)
{
  switch (destination)
  {
    case 1:
      _bodyColor = color;
      for (int i = 1; i < _snakeLength; i++)
      {
        snakeLimbs[i].pixelColor = _bodyColor;
      }
      break;
    case 2:
      _headColor = color;
      snakeLimbs[0].pixelColor = _headColor;
      break;
    case 3:
      _foodColor = color;
      food[0].pixelColor = _foodColor;
      break;
  }
}

void Snake::setDelay(int moveDelay)
{
  _moveDelay = moveDelay;
}
