// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "WordsList.h"
#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    InitGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ProcessGuess(Input);
}

void UBullCowCartridge::ProcessGuess(const FString& PlayerGuess)
{
    if(bGameOver)
    {
        ClearScreen();
        InitGame();
        return;
    }
    if(!IsIsogram(PlayerGuess))
    {
        PrintLine(TEXT("Word can't have repeating letters"));
        return;
    }
    
    if(PlayerGuess == HiddenWord)
    {
        PrintLine(TEXT("Correct word"));
        EndGame();
        return;
    }

    if(PlayerGuess.Len()!=HiddenWord.Len())
    {
        PrintLine(TEXT("Wrong word length. The word length is %i"), HiddenWord.Len());
        return;
    }
    PrintLine(TEXT("Incorrect word"));
    IncorrectGuess();
    
    int32 Bulls, Cows;
    
    GetBullCows(PlayerGuess, Bulls, Cows);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);
}

void UBullCowCartridge::InitGame()
{
    const int WordIndex = FMath::RandRange(0,Words.Num()-1);
    HiddenWord = Words[WordIndex];
    Lives = HiddenWord.Len();
    
    PrintLine(TEXT("Hi there and welcome to my Game!"));
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Guess the %i letter word:"), HiddenWord.Len());
    PrintLine(TEXT("Bull = correct letter and placement"));
    PrintLine(TEXT("Cow = correct letter but not placement"));
    
    bGameOver=false;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Game over. Press enter to play again!"));
}

void UBullCowCartridge::IncorrectGuess()
{
    if(Lives==0)
    {
        PrintLine(TEXT("You have no lives left"));
        EndGame();
        return;
    }
    PrintLine(TEXT("You have %i lives left"),--Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& PlayerGuess)
{
    for (int32 j = 0; j < PlayerGuess.Len(); ++j)
    {
        for (int32 i = j+1; i < PlayerGuess.Len(); ++i)
        {
            if(PlayerGuess[j]==PlayerGuess[i])
                return false;
        }
    }
    return true;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount=0;
    CowCount=0;

    for (int32 Index = 0; Index < Guess.Len(); ++Index)
    {
        if(Guess[Index] == HiddenWord[Index])
        {
            BullCount++;
            continue;
        }
        for (int32 HidIndex = 0; HidIndex < HiddenWord.Len(); ++HidIndex)
        {
            if(Guess[Index] == HiddenWord[HidIndex])
            {
                CowCount++;
            }
        }
    }
}