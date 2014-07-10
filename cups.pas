program CupsProg;
{$mode objfpc}
{$m+}

Uses math, sysutils, contnrs;

type
  CupsPtr = ^Cups;
  StatePtr = ^CupsState;

  Cups = Class
    private
      x, y: Integer;
    public
      constructor new(_x, _y: Integer);
      destructor done();
      function valid(): Boolean;
      function getX(): Integer;
      function getY(): Integer;
      function toString(): String;
      function fillX(): Cups;
      function fillY(): Cups;
      function emptyX(): Cups;
      function emptyY(): Cups;
      function pourXY(): Cups;
      function pourYX(): Cups;
  end;

  CupsState = Class
    private
      steps: Integer;
      parent: CupsPtr;
    public
      constructor new(s: Integer; p: CupsPtr);
      destructor done();
      function getSteps(): Integer;
      function getParent(): CupsPtr;
  end;
var
  pushedNewState: boolean;
  currentCups, nextCups: CupsPtr;
  currentState, nextState: StatePtr;
  hash: TFPHashList;
  queue: TQueue;
  outputStack: TStack;
const
  VOLUME_X = 4;
  VOLUME_Y = 9;
  TARGET_VOLUME = 2;

constructor Cups.new(_x, _y: Integer);
begin
  x := _x;
  y := _y;
end;

destructor Cups.done();
begin
end;

function Cups.valid(): Boolean;
begin
  valid := ((x = TARGET_VOLUME) or (y = TARGET_VOLUME));
end;

function Cups.getX(): Integer;
begin
  getX := x;
end;

function Cups.getY(): Integer;
begin
  getY := y;
end;

function Cups.toString(): String;
begin
  toString := '(' + IntToStr(x) + ', ' + IntToStr(y) + ')';
end;

function Cups.fillX(): Cups;
begin
  fillX := Cups.new(VOLUME_X, y);
end;

function Cups.fillY(): Cups;
begin
  fillY := Cups.new(x, VOLUME_Y);
end;

function Cups.emptyX(): Cups;
begin
  emptyX := Cups.new(0, y);
end;

function Cups.emptyY(): Cups;
begin
  emptyY := Cups.new(x, 0);
end;

function Cups.pourXY(): Cups;
var
  newX, newY: Integer;
begin
  newX := min(VOLUME_X, x + y);
  newY := y - (newX - x);
  pourXY := Cups.new(newX, newY);
end;

function Cups.pourYX(): Cups;
var
  newX, newY: Integer;
begin
  newY := min(VOLUME_Y, x + y);
  newX := x - (newY - y);
  pourYX := Cups.new(newX, newY);
end;

constructor CupsState.new(s: Integer; p: CupsPtr);
begin
  steps := s;
  parent := p;
end;

destructor CupsState.done();
begin
end;

function CupsState.getSteps(): Integer;
begin
  getSteps := steps;
end;

function CupsState.getParent(): CupsPtr;
begin
  getParent := parent;
end;

begin
  new(currentCups);
  new(currentState);
  currentCups^ := Cups.new(0, 0);
  currentState^ := CupsState.new(0, nil);
  hash := TFPHashList.create();
  queue := TQueue.create();

  queue.push(currentCups);
  hash.add(currentCups^.toString(), currentState);

  while queue.count > 0 do
  begin
    currentCups := CupsPtr(queue.pop);
    currentState := StatePtr(hash.find(currentCups^.toString));
    new(nextState);
    nextState^ := CupsState.new(currentState^.getSteps() + 1, currentCups);
    pushedNewState := false;

    if currentCups^.valid then
    begin
      outputStack := TStack.create();
      writeln(currentState^.getSteps());

      while currentCups <> nil do
      begin
        outputStack.push(currentCups);

        currentState := StatePtr(hash.find(currentCups^.toString));
        currentCups := currentState^.getParent();
      end;

      while outputStack.count > 0 do
      begin
        currentCups := CupsPtr(outputStack.pop());
        writeln(currentCups^.toString());
      end;
      break
    end;

    new(nextCups);
    nextCups^ := currentCups^.fillX();
    if hash.find(nextCups^.toString) = nil then
    begin
      queue.push(nextCups);
      hash.add(nextCups^.toString, nextState);
      pushedNewState := true;
    end
    else
    begin
      nextCups^.done();
      dispose(nextCups);
    end;

    new(nextCups);
    nextCups^ := currentCups^.fillY();
    if hash.find(nextCups^.toString) = nil then
    begin
      queue.push(nextCups);
      hash.add(nextCups^.toString, nextState);
      pushedNewState := true;
    end
    else
    begin
      nextCups^.done();
      dispose(nextCups);
    end;

    new(nextCups);
    nextCups^ := currentCups^.emptyX();
    if hash.find(nextCups^.toString) = nil then
    begin
      queue.push(nextCups);
      hash.add(nextCups^.toString, nextState);
      pushedNewState := true;
    end
    else
    begin
      nextCups^.done();
      dispose(nextCups);
    end;

    new(nextCups);
    nextCups^ := currentCups^.emptyY();
    if hash.find(nextCups^.toString) = nil then
    begin
      queue.push(nextCups);
      hash.add(nextCups^.toString, nextState);
      pushedNewState := true;
    end
    else
    begin
      nextCups^.done();
      dispose(nextCups);
    end;

    new(nextCups);
    nextCups^ := currentCups^.pourXY();
    if hash.find(nextCups^.toString) = nil then
    begin
      queue.push(nextCups);
      hash.add(nextCups^.toString, nextState);
      pushedNewState := true;
    end
    else
    begin
      nextCups^.done();
      dispose(nextCups);
    end;

    new(nextCups);
    nextCups^ := currentCups^.pourYX();
    if hash.find(nextCups^.toString) = nil then
    begin
      queue.push(nextCups);
      hash.add(nextCups^.toString, nextState);
      pushedNewState := true;
    end
    else
    begin
      nextCups^.done();
      dispose(nextCups);
    end;

    if not pushedNewState then
    begin
      nextState^.done();
      dispose(nextState);
    end;
  end;
end.
