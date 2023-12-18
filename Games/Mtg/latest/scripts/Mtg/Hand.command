if(msg.box != NULL)
  Msg("Loading {gold}Hand.command");

# ColorCardName(card id) - Return the name of the card, preceded by a color format code.
def ColorCardName
{
  push(color);
  color = Attr("color",ARG);
  if(find(" ",color) != NULL)
    return(" {gold}{card"+ARG+"}{reset},");
  else if(color == "White")
    return(" {white}{card"+ARG+"}{reset},");
  else if(color == "Blue")
    return(" {blue}{card"+ARG+"}{reset},");
  else if(color == "Black")
    return(" {gray}{card"+ARG+"}{reset},");
  else if(color == "Red")
    return(" {red}{card"+ARG+"}{reset},");
  else if(color == "Green")
    return(" {green}{card"+ARG+"}{reset},");
  else if(color == "Artifact")
    return(" {154,154,154}{card"+ARG+"}{reset},");
  else if(color == "Land")
    return(" {154,128,77}{card"+ARG+"}{reset},");
  else if(color == "Colorless")
    return(" {128,128,128}{card"+ARG+"}{reset},");
  else return " {card"+ARG+"},";
  color=pop();
}

def CommandHand
{
  push(i);
  push(h);
  h="{orange}Sample hand:";
  if(length(ARG)<1) deckpos = HANDSIZE;
  else deckpos = toint(ARG[0]);
  sampledeck=shuffle(decks{deck.name}{"deck"});
  
  if (deckpos > length(sampledeck))
    Msg("{red}Not enough cards in the deck.");
  else if (deckpos < 1)
    Msg("{red}Nothing to do.");
  else
  {
    for(i)(deckpos)
      h=h+ColorCardName(sampledeck[i]);
    Msg(left(h,length(h)-1)+".");
  }
  h=pop();
  i=pop();
}

def CommandNextcard
{
  push(i);
  push(h);
  h="{orange}You drew";
  if(length(ARG)<1) ARG = 1;
  else ARG = toint(ARG[0]);
  
  if (deckpos < 1)
    Msg("{red}Draw a hand first.");
  else if (deckpos + ARG > length(sampledeck))
    Msg("{red}Not enough cards in the deck.");
  else if (ARG < 1)
    Msg("{red}Nothing to do.");
  else
  {
    for(i)(ARG)
      h=h+ColorCardName(sampledeck[deckpos+i]);
    Msg(left(h,length(h)-1)+".");
    deckpos = deckpos+ARG;
  }
  h=pop();
  i=pop();
}

HELP{"any"}{"hand"}=("[n]","generate sample hand",NULL,
"Shuffle up the deck and deal out a sample hand of {yellow}n{white} cards, for testing your deck's consistency without having to go to a table. If not specified, {yellow}n{white} is 7. To deal additional cards, use {yellow}/nextcard{white}");

HELP{"any"}{"nextcard"}=("[n]","draw more cards to go with sample hand",NULL,
"After dealing out a sample hand with {yellow}/hand{white}, you can use this to draw additional cards as a continuation. If not specified, {yellow}n{white} is 1.");
