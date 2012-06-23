/**
 * Provides the MovementSystem class for handing all entity movement in a game.
 *
 * @file src/GQE/Entity/systems/MovementSystem.cpp
 * @author Jacob Dix
 * @date 20120611 - Initial Release
 * @date 20120616 - Adjustments for new PropertyManager class
 * @date 20120618 - Use IEntity not Instance and changed AddPrototype to AddProperties
 * @date 20120622 - Small adjustments to implementation and Handle methods
 * @date 20120623 - Improved documentation and adjusted some properties
 */
#include <SFML/Graphics.hpp>
#include <GQE/Entity/systems/MovementSystem.hpp>
#include <GQE/Entity/interfaces/IEntity.hpp>

namespace GQE
{
  MovementSystem::MovementSystem(IApp& theApp):
    ISystem("MovementSystem",theApp)
  {
  }
  MovementSystem::~MovementSystem()
  {
  }

  void MovementSystem::AddProperties(IEntity* theEntity)
  {
    theEntity->mProperties.Add<sf::Vector2f>("Velocity",sf::Vector2f(0,0));
    theEntity->mProperties.Add<sf::Vector2f>("Acceleration",sf::Vector2f(0,0));
    theEntity->mProperties.Add<float>("RotationalVelocity",0);
    theEntity->mProperties.Add<float>("RotationalAcceleration",0);
    theEntity->mProperties.Add<bool>("FixedMovement",true);
    theEntity->mProperties.Add<bool>("ScreenWrap",true);
    theEntity->AddSystem(this);
  }

  void MovementSystem::HandleInit(IEntity* theEntity)
  {
    // Do nothing
  }

  void MovementSystem::HandleEvents(sf::Event theEvent)
  {
  }

  void MovementSystem::UpdateFixed()
  {
    std::map<const typeEntityID, IEntity*>::iterator anEntityIter = mEntities.begin();

    // Loop through each IEntity in our mEntities map
    while(anEntityIter != mEntities.end())
    {
      // Get the IEntity address first
      IEntity* anEntity = anEntityIter->second;

      // Increment the IEntity iterator second
      anEntityIter++;

      // Are we using fixed movement mathematics?
      if(anEntity->mProperties.Get<bool>("FixedMovement"))
      {
        // Get the RenderSystem properties
        sf::Vector2f anPosition = anEntity->mProperties.Get<sf::Vector2f>("Position");
        float anRotation = anEntity->mProperties.Get<float>("Rotation");

        // Get the MovementSystem properties
        sf::Vector2f anVelocity = anEntity->mProperties.Get<sf::Vector2f>("Velocity");
        sf::Vector2f anAccelleration = anEntity->mProperties.Get<sf::Vector2f>("Acceleration");
        float anRotationalVelocity = anEntity->mProperties.Get<float>("RotationalVelocity");
        float anRotationalAccelleration = anEntity->mProperties.Get<float>("RotationalAcceleration");

        // Now update the current movement properties
        anVelocity += anAccelleration;
        anPosition += anVelocity;
        anRotationalVelocity += anRotationalAccelleration;
        anRotation += anRotationalVelocity;

        // If ScreenWrap is true, account for screen wrapping
        if(anEntity->mProperties.Get<bool>("ScreenWrap"))
        {
          if(anPosition.x > (float)mApp.mWindow.getSize().x)
          {
            anPosition.x = 0.0f;
          }
          else if(anPosition.x < 0.0f)
          {
            anPosition.x = (float)mApp.mWindow.getSize().x;
          }
          if(anPosition.y > (float)mApp.mWindow.getSize().y)
          {
            anPosition.y = 0.0f;
          }
          else if(anPosition.y < 0.0f)
          {
            anPosition.y = (float)mApp.mWindow.getSize().y;
          }
        }

        // Now update the MovementSystem properties for this IEntity class
        anEntity->mProperties.Set<sf::Vector2f>("Velocity",anVelocity);
        anEntity->mProperties.Set<float>("RotationalVelocity",anRotationalVelocity);

        // Now update the RenderSystem properties of this IEntity class
        anEntity->mProperties.Set<sf::Vector2f>("Position",anPosition);
        anEntity->mProperties.Set<float>("Rotation",anRotation);
      } //if(anEntity->mProperties.Get<bool>("FixedMovement"))
    } // while(anEntityIter != mEntities.end())
  }

  void MovementSystem::UpdateVariable(float theElapsedTime)
  {
    std::map<const typeEntityID, IEntity*>::iterator anEntityIter = mEntities.begin();

    // Loop through each IEntity in our mEntities map
    while(anEntityIter != mEntities.end())
    {
      // Get the IEntity address first
      IEntity* anEntity = anEntityIter->second;

      // Increment the IEntity iterator second
      anEntityIter++;

      // Are we NOT using fixed movement mathematics?
      if(anEntity->mProperties.Get<bool>("FixedMovement") == false)
      {
        // Get the RenderSystem properties
        sf::Vector2f anPosition = anEntity->mProperties.Get<sf::Vector2f>("Position");
        float anRotation = anEntity->mProperties.Get<float>("Rotation");

        // Get the MovementSystem properties
        sf::Vector2f anVelocity = anEntity->mProperties.Get<sf::Vector2f>("Velocity");
        sf::Vector2f anAccelleration = anEntity->mProperties.Get<sf::Vector2f>("Acceleration");
        float anRotationalVelocity = anEntity->mProperties.Get<float>("RotationalVelocity");
        float anRotationalAccelleration = anEntity->mProperties.Get<float>("RotationalAcceleration");

        // Now update the current movement properties
        anVelocity += anAccelleration * theElapsedTime;
        anPosition += anVelocity * theElapsedTime;
        anRotationalVelocity += anRotationalAccelleration * theElapsedTime;
        anRotation += anRotationalVelocity * theElapsedTime;

        // If ScreenWrap is true, account for screen wrapping
        if(anEntity->mProperties.Get<bool>("ScreenWrap"))
        {
          if(anPosition.x > (float)mApp.mWindow.getSize().x)
          {
            anPosition.x = 0.0f;
          }
          else if(anPosition.x < 0.0f)
          {
            anPosition.x = (float)mApp.mWindow.getSize().x;
          }
          if(anPosition.y > (float)mApp.mWindow.getSize().y)
          {
            anPosition.y = 0.0f;
          }
          else if(anPosition.y < 0.0f)
          {
            anPosition.y = (float)mApp.mWindow.getSize().y;
          }
        }

        // Now update the MovementSystem properties for this IEntity class
        anEntity->mProperties.Set<sf::Vector2f>("Velocity",anVelocity);
        anEntity->mProperties.Set<float>("RotationalVelocity",anRotationalVelocity);

        // Now update the RenderSystem properties of this IEntity class
        anEntity->mProperties.Set<sf::Vector2f>("Position",anPosition);
        anEntity->mProperties.Set<float>("Rotation",anRotation);
      } //if(anEntity->mProperties.Get<bool>("FixedMovement") == false)
    } // while(anEntityIter != mEntities.end())
  }

  void MovementSystem::Draw()
  {
  }

  void MovementSystem::HandleCleanup(IEntity* theEntity)
  {
    // Do nothing
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Jacob Dix
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
