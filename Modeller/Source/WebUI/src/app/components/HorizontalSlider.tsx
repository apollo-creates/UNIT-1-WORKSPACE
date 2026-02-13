import { useState, useRef, useEffect } from 'react';

interface HorizontalSliderProps {
  initialValue?: number; // 0 to 1
  onChange?: (value: number) => void;
  trackWidth?: number;
}

export function HorizontalSlider({ 
  initialValue = 0.5, 
  onChange,
  trackWidth = 160 
}: HorizontalSliderProps) {
  const [value, setValue] = useState(initialValue);
  const [isDragging, setIsDragging] = useState(false);
  const trackRef = useRef<HTMLDivElement>(null);

  const handleMouseDown = (e: React.MouseEvent) => {
    e.preventDefault();
    setIsDragging(true);
    updatePosition(e.clientX);
  };

  const updatePosition = (clientX: number) => {
    if (!trackRef.current) return;

    const rect = trackRef.current.getBoundingClientRect();
    const handleWidth = 15; // Width of the slider handle
    const maxX = trackWidth - handleWidth;
    
    let newX = clientX - rect.left - handleWidth / 2;
    newX = Math.max(0, Math.min(newX, maxX));
    
    const newValue = newX / maxX;
    setValue(newValue);
    onChange?.(newValue);
  };

  useEffect(() => {
    const handleMouseMove = (e: MouseEvent) => {
      if (!isDragging) return;
      updatePosition(e.clientX);
    };

    const handleMouseUp = () => {
      setIsDragging(false);
    };

    if (isDragging) {
      document.addEventListener('mousemove', handleMouseMove);
      document.addEventListener('mouseup', handleMouseUp);
    }

    return () => {
      document.removeEventListener('mousemove', handleMouseMove);
      document.removeEventListener('mouseup', handleMouseUp);
    };
  }, [isDragging]);

  const handlePosition = value * (trackWidth - 15);

  return (
    <div className="absolute left-[1301px] top-[394px]">
      {/* Track */}
      <div 
        ref={trackRef}
        className="absolute bg-[#202020] h-[6px] rounded-[5px] top-[14px] cursor-pointer"
        style={{ width: `${trackWidth}px` }}
        onMouseDown={handleMouseDown}
      >
        <div className="absolute inset-0 pointer-events-none rounded-[inherit] shadow-[inset_0px_4px_4px_0px_rgba(0,0,0,0.25)]" />
      </div>
      
      {/* Handle */}
      <div 
        className="absolute bg-[#232323] blur-[0.25px] border border-[#1f1f1f] border-solid h-[34px] rounded-[5px] top-0 w-[15px] cursor-grab active:cursor-grabbing"
        style={{ left: `${handlePosition}px` }}
        onMouseDown={handleMouseDown}
      >
        <div className="absolute bg-[#d9d9d9] h-[28px] left-[7px] top-[3px] w-px" />
      </div>
    </div>
  );
}
